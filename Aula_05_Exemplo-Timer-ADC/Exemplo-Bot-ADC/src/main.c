/* Envia valor da conversão AD do potenciômetro (AD5) em intervalos programados pela interrupção
   do TC0 (timer 0 canal 0), conta o número de vezes que o botão BP2 é pressionado (borda de subida)
   e envia os resultados para o LCD
*/
#include <asf.h>

//protótipos de funções
void configure_lcd(void);
void configure_botao(void);
void configure_adc(void);
static void tc_config(uint32_t freq_desejada);

/** Chip select number to be set */
#define ILI93XX_LCD_CS      1

/** Reference voltage for ADC,in mv. */
#define VOLT_REF        (3300)
/* Tracking Time*/
#define TRACKING_TIME    15
/* Transfer Period */
#define TRANSFER_PERIOD  2
/* Startup Time*/
#define STARTUP_TIME ADC_STARTUP_TIME_4

/** The maximal digital value */
#define MAX_DIGITAL     (4095)
#define ADC_CHANNEL 5

struct ili93xx_opt_t g_ili93xx_display_opt;

#define TC			TC0			//USANDO TIMER 0
#define CHANNEL		0			//canal 0 do timer
#define ID_TC		ID_TC0		// ID do periférico TC0/canal 0 é ID_TC0 = 23
#define TC_Handler  TC0_Handler //define como TC_Handler a rotina de interrupção do timer 
#define TC_IRQn     TC0_IRQn	// ID do timer 0 canal 0 para geração de interrupção (23)

/**
 *  Configure Timer Counter 0 to generate an interrupt every ...freq_desejada (Hz)
 */
static void tc_config(uint32_t freq_desejada)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t counts;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();  //retorna a frequência atual do clock da CPU em Hz
	
	pmc_enable_periph_clk(ID_TC);   //habilita o clock para o módulo timer (no caso timer 0)
	
	/* -encontra o melhor MCK divisor
	   -retorna 1 se encontrou o divisor
	   -freq_desejada : frequência desejada do timer em Hz (IN)
	   -ul_sysclk: frequência atual do clock da CPU em Hz (IN)
	   -ul_div: valor do divisor { 2, 8, 32, 128, BOARD_MCK/32768} (out) (tabela 37.1 datasheet) 
	   -ul_tcclks: valor do campo TCCLKS para o valor do divisor no TC Channel Mode Register: Capture Mode (OUT)   
	   -BOARD_MCK: clock máximo da CPU/placa = 120000000Hz (120MHz) 120000000UL
	*/
	
	tc_find_mck_divisor( freq_desejada, ul_sysclk, &ul_div, &ul_tcclks,	BOARD_MCK); 
	
	/*configura TC para modo compare 
	  -TC = TC0 , canal 0,  		
	  - Programa o registrador "TC Channel Mode Register" do canal 0 (TC_CMR0) para
	   o modo RC Compare Trigger Enable (seta bit 14) no registrador TC_CMR0 e programa o ul_tcclks no 
	   mesmo registrador fazendo um OR: ul_tcclks | TC_CMR_CPCTRG. OBS: TC_CMR_CPCTRG =(0x1u << 14)
	   OBS: modo RC Compare Trigger Enable: compara o valor da contagem com RC e se igual resta o contador e inicia a contagem de clock.
	*/																			
	tc_init(TC, CHANNEL, ul_tcclks | TC_CMR_CPCTRG);
	
	/*calcula o valor de contagem que devemos colocar no registardor RC do timer para fornecer a frequência desejada
	  counts = (freq_cpu/divisor selecionado)/ frequência desejada
	*/
	counts = (ul_sysclk/ul_div)/freq_desejada;
	
	tc_write_rc(TC, CHANNEL, counts);  //escreve valor no registro RC do timer e canal utilizados.

	NVIC_ClearPendingIRQ(TC_IRQn); //limpa interupção do timer (ID d timer e canal utilizados = 23)
	NVIC_SetPriority(TC_IRQn, 4); // prioridade 4 para a interrupção do timer (botão tem prioridade 5, então timer pode 
								  //interromper a rotina de interrupção do botão						
	NVIC_EnableIRQ(TC_IRQn);      // habilita interupção do timer no NVIC
	
	// Enable interrupts for this TC, and start the TC.
	tc_enable_interrupt(TC,	CHANNEL, TC_IER_CPCS); // habilita interrupção timer 0 canal 0 e habilita interrupção
												   // quando o valor da contagem for igual ao valor do registrador RC
												   // programado (TC_IER_CPCS)
	tc_start(TC, CHANNEL); //dispara a contagem do timer counter no timer e canal programados.
}



/************************************************************************/
/* HANDLERS                                                             */
/************************************************************************/
static void push_button_handle(uint32_t id, uint32_t mask)
{
	//adc_start(ADC);  //dispara conversão AD pelo botão
	static unsigned int contagem =0;
	char buffer[10];
	sprintf (buffer, "%d", contagem);  //monta string
	
	// Desenha retângulo
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(135, 250, 240, 275);
	
	// Escreve uma String no LCD na posição 140, 255
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(140, 255, (uint8_t*) buffer);
	contagem++;	  //incrementa contador de liberação do botão
}

// Interrupt service routine - INTERRUPÇÃO do TIMER
void TC_Handler(void)
{
	tc_get_status(TC,CHANNEL);  //lê status do canal do timer, limpando o bit que indica que a contagem = registro RC
							    //preparando para a próxima interrupção
	adc_start(ADC);				//dispara conversão AD
}


/**
* \brief ADC interrupt handler.
*/
void ADC_Handler(void)
{
	uint16_t result;

	if ((adc_get_status(ADC) & ADC_ISR_DRDY) == ADC_ISR_DRDY)
	{
		// Recupera o último valor da conversão
		result = adc_get_latest_value(ADC);
		
		char buffer[10];
		sprintf (buffer, "%d", result);  //monta string
		
		// Desenha retângulo
		ili93xx_set_foreground_color(COLOR_WHITE);
		ili93xx_draw_filled_rectangle(135, 175, 240, 200);
		
		// Escreve uma String no LCD na posição 140, 180
		ili93xx_set_foreground_color(COLOR_BLACK);
		ili93xx_draw_string(140, 180, (uint8_t*) buffer); 
	}
}

/************************************************************************/
/* CONFIGs                                                              */
/************************************************************************/
void configure_lcd()
{
	/** Enable peripheral clock */
	pmc_enable_periph_clk(ID_SMC);

	/** Configure SMC interface for Lcd */
	smc_set_setup_timing(SMC, ILI93XX_LCD_CS, SMC_SETUP_NWE_SETUP(2)
	| SMC_SETUP_NCS_WR_SETUP(2)
	| SMC_SETUP_NRD_SETUP(2)
	| SMC_SETUP_NCS_RD_SETUP(2));
	
	smc_set_pulse_timing(SMC, ILI93XX_LCD_CS, SMC_PULSE_NWE_PULSE(4)
	| SMC_PULSE_NCS_WR_PULSE(4)
	| SMC_PULSE_NRD_PULSE(10)
	| SMC_PULSE_NCS_RD_PULSE(10));
	
	smc_set_cycle_timing(SMC, ILI93XX_LCD_CS, SMC_CYCLE_NWE_CYCLE(10)
	| SMC_CYCLE_NRD_CYCLE(22));
	
	smc_set_mode(SMC, ILI93XX_LCD_CS, SMC_MODE_READ_MODE
	| SMC_MODE_WRITE_MODE);

	/** Initialize display parameter */
	g_ili93xx_display_opt.ul_width = ILI93XX_LCD_WIDTH;
	g_ili93xx_display_opt.ul_height = ILI93XX_LCD_HEIGHT;
	g_ili93xx_display_opt.foreground_color = COLOR_BLACK;
	g_ili93xx_display_opt.background_color = COLOR_WHITE;

	/** Switch off backlight */
	aat31xx_disable_backlight();

	/** Initialize LCD */
	ili93xx_init(&g_ili93xx_display_opt);

	/** Set backlight level */
	aat31xx_set_backlight(AAT31XX_AVG_BACKLIGHT_LEVEL);

	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(0, 0, ILI93XX_LCD_WIDTH,
	ILI93XX_LCD_HEIGHT);
	/** Turn on LCD */
	ili93xx_display_on();
	ili93xx_set_cursor_position(0, 0);
}

void configure_botao(void)
{
	pmc_enable_periph_clk(ID_PIOB);
	
	pio_set_input(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR ,push_button_handle);//borda de subida
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);
	NVIC_SetPriority( PIN_PUSHBUTTON_1_ID, 5);
	NVIC_EnableIRQ( PIN_PUSHBUTTON_1_ID);
}


void configure_adc(void)
{
	/* Enable peripheral clock. */
	pmc_enable_periph_clk(ID_ADC);
	
	/* Initialize ADC. */
	/*
	* Formula: ADCClock = MCK / ( (PRESCAL+1) * 2 )
	* For example, MCK = 64MHZ, PRESCAL = 4, then:
	* ADCClock = 64 / ((4+1) * 2) = 6.4MHz;
	*/
	/* Formula:
	*     Startup  Time = startup value / ADCClock
	*     Startup time = 64 / 6.4MHz = 10 us
	*/
	adc_init(ADC, sysclk_get_cpu_hz(), 6400000, STARTUP_TIME);
	
	
	/* Formula:
	*     Transfer Time = (TRANSFER * 2 + 3) / ADCClock
	*     Tracking Time = (TRACKTIM + 1) / ADCClock
	*     Settling Time = settling value / ADCClock
	*
	*     Transfer Time = (1 * 2 + 3) / 6.4MHz = 781 ns
	*     Tracking Time = (1 + 1) / 6.4MHz = 312 ns
	*     Settling Time = 3 / 6.4MHz = 469 ns
	*/
	adc_configure_timing(ADC, TRACKING_TIME	, ADC_SETTLING_TIME_3, TRANSFER_PERIOD);

	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);

	/* Enable channel for potentiometer. */
	adc_enable_channel(ADC, ADC_CHANNEL);

	NVIC_SetPriority(ADC_IRQn, 5);
	/* Enable ADC interrupt. */
	NVIC_EnableIRQ(ADC_IRQn);

	/* Start conversion. */
	//adc_start(ADC);

	/* Enable ADC channel interrupt. */
	adc_enable_interrupt(ADC, ADC_IER_DRDY);
}


/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void)
{
	sysclk_init();
	board_init();

	configure_lcd();
	configure_botao();
	configure_adc();
	tc_config(10);  //gera interrupção a cada 10 Hz (100 ms)

	/** Draw text, image and basic shapes on the LCD */
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(10, 20, (uint8_t *)"Aula-ADC com Timer"); //escreve título
	ili93xx_draw_string(10, 180, (uint8_t *)"Conversor"); //escreve label fixo para valor da conversão AD
	ili93xx_draw_string(10, 255, (uint8_t *)"Contagem"); //escreve label fixo para valor da contagem do botão

	while (1) {
	}
}

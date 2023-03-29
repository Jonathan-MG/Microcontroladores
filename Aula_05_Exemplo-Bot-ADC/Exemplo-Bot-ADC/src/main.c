// Conversão AD do valor do potenciômetro do kit usando interrupção do botão para e enviando resultado para display

#include <asf.h>

//Protótipos das funções 
void configure_lcd(void);
void configure_botao(void);
void configure_adc(void);

/** Chip select number to be set */
#define ILI93XX_LCD_CS      1

/** Reference voltage for ADC,in mv. */
#define VOLT_REF        (3300)
/* Tracking Time*/
#define TRACKING_TIME    15		//usado em adc_configure_timing
/* Transfer Period */
#define TRANSFER_PERIOD  2		//usado em adc_configure_timing (hold time: seguindo manual deve ser colocado 2 no registro)
/* Startup Time*/
#define STARTUP_TIME ADC_STARTUP_TIME_4  //usado em adc_init()
/** The maximal digital value */
#define MAX_DIGITAL     (4095)

#define ADC_CHANNEL 5  // potenciômetro está ligado no PB1 que é canal 5

struct ili93xx_opt_t g_ili93xx_display_opt;

/************************************************************************/
/* HANDLERS                                                            */
/************************************************************************/
static void push_button_handle(uint32_t id, uint32_t mask)
{
	adc_start(ADC); //dispara conversão por software
}


/**
* \brief ADC interrupt handler.
*/
void ADC_Handler(void)
{
	uint16_t result;
	
    //lê ADC_ISR, registro de status da interrupção e vê se a interrupção foi por data ready
	if ((adc_get_status(ADC) & ADC_ISR_DRDY) == ADC_ISR_DRDY) 
	{
		result = adc_get_latest_value(ADC); // Recupera o último valor da conversão (inteiro não sinalizadoa0
		
		/* sprintf monta uma string no array buffer com o mesmo formato que seria enviado ao console se usando printf. 
		O tamanho de buffer deve ser grande suficiente para o armazenamento da string
		Um caracter null é inserido no fim do conteúdo e após o parâmetro de formato a funcção espera os argumentos necessários
		é uma função do "stdio.h"
		*/
		
		char buffer[10];
		sprintf (buffer, "%d", result);  //transforma result em uma string formatada para escrver no display
		
		// Desenha retângulo
		ili93xx_set_foreground_color(COLOR_WHITE);
		ili93xx_draw_filled_rectangle(135, 175, 240, 200);
		
		// Escreve uma String no LCD na posição 140, 180
		ili93xx_set_foreground_color(COLOR_BLACK);
		ili93xx_draw_string(140, 180, (uint8_t*) buffer);  //escreve a string no display
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
	// Usando funções do driver PIO. Pederia ser utilizada a API do serviço IOPORT. OBS: Use ALT+G para ver as ctes
	pmc_enable_periph_clk(ID_PIOB);  // habilita 
	
	// botão no PB3 (BP2 ou USRPB1), ativando pullup, debounce e interrupção por borda de subida
	pio_set_input(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR);
	
	//configura debouncing
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);//programa tempo de debouncing
	
	// interrupção do botão no PB3 do PIO B será por borda de subida e irá chamar a rotina "push_button_handle"
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR ,push_button_handle);
	
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK); // habilita interrupção do PB3 do PIOB
	NVIC_SetPriority( PIN_PUSHBUTTON_1_ID, 5); //seta prioridade 5 para a interrupção PB3
	NVIC_EnableIRQ( PIN_PUSHBUTTON_1_ID); // habilita interrupção do PB3
}


void configure_adc(void)
{
	/* Enable peripheral clock. */
	pmc_enable_periph_clk(ID_ADC);   //libera o clock para o A/D no PMC
	
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
	adc_init(ADC, sysclk_get_cpu_hz(), 6400000, STARTUP_TIME); //inicia ADC (ponteiro para o ADC, clock principal da cpu,
														//clock de conversão AD em Hz (ADC_CLK = 6400000), ADC startup time
														// STARTUP_TIME = 4 => SUT64 64 periods of ADCCLK (ADC Mode register)
	/* Formula:
	*     Transfer Time = (TRANSFER * 2 + 3) / ADCClock
	*     Tracking Time = (TRACKTIM + 1) / ADCClock
	*     Settling Time = settling value / ADCClock
	*
	*     Transfer Time = (1 * 2 + 3) / 6.4MHz = 781 ns -- (2*2+3)/6.4MHz = 1.1 us
	*     Tracking Time = (1 + 1) / 6.4MHz = 312 ns -- (15+1)/6.4MHz = 2.5us
	*     Settling Time = 3 / 6.4MHz = 469 ns
	*/
	adc_configure_timing(ADC, TRACKING_TIME	, ADC_SETTLING_TIME_3, TRANSFER_PERIOD); //configura ADC Ver ADC mode register
	// obs: ADC_SETTLING_TIME_3 = 3 =>AST17 =>17 periods of ADCCLK no ADC mode register
	
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0); //configura disparo do ADC por software 
												// no ADC Mode regidter (ADC_TRIG_SW) e desabilita modo free running

	/* Enable channel for potentiometer. */
	adc_enable_channel(ADC, ADC_CHANNEL);  // habilita conversão do canl 5 do ADC

	NVIC_SetPriority(ADC_IRQn, 5);	// seta prioridade 5 para o ADC
	/* Enable ADC interrupt. */
	NVIC_EnableIRQ(ADC_IRQn); // habilita interrupção do ADC (ID = 29)

	/* Enable ADC channel interrupt. */
	adc_enable_interrupt(ADC, ADC_IER_DRDY);  //habilita interrupção do ADC 
											  //no registro ADC_IER quando tivermos dado pronto (DRDY)
}


/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void)
{
	sysclk_init();
	board_init();

	configure_lcd();
	configure_adc();
	configure_botao();

	/** Draw text on the LCD */
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(10, 20, (uint8_t *)"Aula - ADC");

	while (1) {
	}
}

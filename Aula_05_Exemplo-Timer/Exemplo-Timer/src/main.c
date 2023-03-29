//Pisca o led azul do kit ba frequência detrminada pela interrupção do timer 0 canal 0
 
#include <asf.h>

//protótipos
void inicializacao_UART (void);
static void tc_config(uint32_t freq_desejada);

#define TC			TC0			//USANDO TIMER 0
#define CHANNEL		0			//canal 0 do timer
#define ID_TC		ID_TC0		// ID do periférico TC0/canal 0 é ID_TC0 = 23
#define TC_Handler  TC0_Handler	//define como TC_Handler a rotina de interrupção do timer 0 canal 0
#define TC_IRQn     TC0_IRQn	// ID do timer 0 canal 0 para geração de interrupção (23)

#define CONF_UART              UART0
#define CONF_UART_BAUDRATE     9600
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT
#define CONF_UART_PARITY       US_MR_PAR_NO
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT

void inicializacao_UART (){
	
	static usart_serial_options_t usart_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits = CONF_UART_STOP_BITS
	};
	
	usart_serial_init((Usart *)CONF_UART, &usart_options);
	
	stdio_serial_init((Usart *)CONF_UART, &usart_options);
}

/**
 *  Configure Timer Counter 0 to generate an interrupt every ...freq_desejada (Hz)
 */
static void tc_config(uint32_t freq_desejada)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t counts;
	uint32_t ul_sysclk = sysclk_get_cpu_hz(); //retorna a frequência atual do clock da CPU em Hz
	
	pmc_enable_periph_clk(ID_TC0);	//habilita o clock para o módulo timer (no caso timer 0)
	
	/* -encontra o melhor MCK divisor
	   -retorna 1 se encontrou o divisor
	   -freq_desejada : frequência desejada do timer em Hz (IN)
	   -ul_sysclk: frequência atual do clock da CPU em Hz (IN)
	   -ul_div: valor do divisor { 2, 8, 32, 128, ul_sysclk/32768} (out) (tabela 37.1 datasheet) 
	   -ul_tcclks: valor do campo TCCLKS para o valor do divisor no TC Channel Mode Register: Capture Mode (OUT)   
	*/
	
	tc_find_mck_divisor( freq_desejada, ul_sysclk, &ul_div, &ul_tcclks,	ul_sysclk);
	
	printf("ul_sysclk: %u \n\r", (unsigned int)ul_sysclk);
	printf("ul_div: %u \n\r", (unsigned int)ul_div);
	printf("ul_tcclks: %u \n\r", (unsigned int)ul_tcclks);
	
	/*configura TC para modo compare 
	  -TC = TC0 , canal 0,  		
	  - Programa o registrador "TC Channel Mode Register" do canal 0 (TC_CMR0) para
	   o modo RC Compare Trigger Enable (seta bit 14) no registrador TC_CMR0 e programa o ul_tcclks no 
	   mesmo registrador fazendo um OR: ul_tcclks | TC_CMR_CPCTRG. OBS: TC_CMR_CPCTRG =(0x1u << 14)
	   OBS: modo RC Compare Trigger Enable: compara o valor da contagem com RC e se igual resta o contador e inicia a contagem de clock.
	*/
	
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);

	//obtém o clock atual em Hz do "Peripheral Bus clock" (clock de periférico) conectado ao canal 0 do timer 0.
	unsigned int clock_timer = sysclk_get_peripheral_bus_hz(TC0); 
	printf("Clock do periferico SCLK: %u \n\r", clock_timer);
	
	/*calcula o valor de contagem que devemos colocar no registardor RC do timer para fornecer a frequência desejada
	  counts = (freq_cpu/divisor selecionado)/ frequência desejada
	*/
	counts = (ul_sysclk/ul_div)/freq_desejada;
	
	printf("counts: %u \n\r", (unsigned int)counts);
	tc_write_rc(TC0, 0, counts);	//escreve valor no registro RC do timer e canal utilizados.

	NVIC_DisableIRQ(TC0_IRQn);
	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_SetPriority(TC0_IRQn,2);
	NVIC_EnableIRQ(TC0_IRQn);
	
	// Enable interrupts for this TC, and start the TC.
	// habilita interrupção timer 0 canal 0 e habilita interrupção
	// quando o valor da contagem for igual ao valor do registrador RC
	// programado (TC_IER_CPCS)
	tc_enable_interrupt(TC,	CHANNEL,TC_IER_CPCS);  // Enable interrupt.
	
	//dispara a contagem do timer counter no timer e canal programados.
	tc_start(TC,CHANNEL);			// Start the TC.
}


// Interrupt service routine
void TC0_Handler(void)
{
	tc_get_status(TC0,0);				//lê status do canal do timer, limpando o bit que indica que a contagem = registro RC
										//preparando para a próxima interrupção
	puts("Executando ISR do timer.\n\r");//manda mensagem pela serial
	LED_Toggle(LED0_GPIO);				// inverte estado do led azul
}


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();

	inicializacao_UART();
	tc_config(1);		//configura timer (no caso timer 0 canal 0) para interromper com frequência de 1 Hz (a cada segundo)
	
	//Configura os PIOs de saida (LEDs) usando as funcções do PIO (driver)
	pio_set_output(PIOA, PIO_PA19, LOW, DISABLE, ENABLE); //inicia com led azul ligado
	pio_set_output(PIOA, PIO_PA20, HIGH, DISABLE, ENABLE);// led verde desligado
	
	while(1) {
		
	}
	
}

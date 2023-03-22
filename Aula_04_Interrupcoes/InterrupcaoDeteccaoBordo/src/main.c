/* Gabarito - Interrup��es PIO */
#include <asf.h>
#include "string.h" //obs: n�o necess�rio nesta aplica��o

//prot�tipos das fun��es

void inicializacao_UART (void);
void tratamento_interrupcao_pioB(const uint32_t id, const uint32_t index);
void configurar_botao1 (void);


#define CONF_UART              UART0
#define CONF_UART_BAUDRATE     115200
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT
#define CONF_UART_PARITY       US_MR_PAR_NO
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT


#define BT3 PIO_PB3   // usa bot�o BP2 (USRPB1) ligado no pino 3 do PIOB 

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


void tratamento_interrupcao_pioB(const uint32_t id, const uint32_t index) // rotina de interrup��o
{
	puts("\n\rEntrando - Interrupcao B\r");
	puts("*Parametros entrada\r");
	printf( "****id = %u \n\r", (unsigned int)id );		  // o id � a inst�ncia (perif�rico) que interrompeu que para o PIOB � 12
	printf( "****index = %u \n\r", (unsigned int)index ); // o index � o pino que interrompeu em decimal. No caso pino 3 d� 8 em decimal
	
	LED_Toggle(LED0_GPIO);						// esta � uma fun��o de alto n�vel que vem no ambiente da Atmel pois carregamos um kit conhecido
												// LED0_GPIO � o pino 19 do PIOA --> Led azul
	
}


void configurar_botao1 (){
	
	pmc_enable_periph_clk(ID_PIOB); // habilita o clock do perif�rico PIOB (para usar debouncing)
	pio_set_input(PIOB, PIO_PB3, PIO_PULLUP | PIO_DEBOUNCE);  // configura pino 3 do PIOB como entrada (PIO_PB3 = 1<<3), ativa pull up e 
															  // deboncing (fun��o API PIO). 
															  
	NVIC_DisableIRQ(PIOB_IRQn);		// desabilita a interrup��o no NVIC do PIOB que tem ID =12 (inst�ncia id PIOB = PIOB_IRQn=12) 
															   
	// configura interrup��o do PIOB (int�ncia 12 = ID_PIOB), Pino 3 , (PIO_PB3),borda de subida (PIO_IT_RISE_EDGE). 
	//Quando isto ocorrer chama a rotina especificada. No caso atribuiu-se o nome: "tratamento_interrupcao_pioB()" 		
													  
	pio_handler_set(PIOB, ID_PIOB, PIO_PB3, PIO_IT_RISE_EDGE, tratamento_interrupcao_pioB);
	
	
	pio_enable_interrupt(PIOB, PIO_PB3); //habilita interrup��o do pino 3 do PIOB no registrador PIO_IER do PIOB (fun��o da API PIO)
	
	NVIC_ClearPendingIRQ(PIOB_IRQn); // limpa interrup��es pendentes do PIOB
	NVIC_SetPriority(PIOB_IRQn,15);  // seta a prioridade da interrup��o no controlador de interrup��o (NVIC) como 15
	NVIC_EnableIRQ(PIOB_IRQn);       // habilita a interrup��o no NVIC do PIOB
}



int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	
	inicializacao_UART();
	configurar_botao1();

	//Configura os PIOs de saida (LEDs)
	pio_set_output(PIOA, PIO_PA19, LOW, DISABLE, ENABLE); // fun��o do PIO ASF: configura o pino 19 do PIOA como sa�da, n�vel baixo, sa�da n�o
														  // � dreno aberto, e ativa pull up no pino
	pio_set_output(PIOA, PIO_PA20, LOW, DISABLE, ENABLE); // idem para pino 20 do PIOA
	
	while (1)
	{
        /**
         * Entra em modo Sleep 
         * s� � acordado por uma interrup��o
         */
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI); //manda o processador para modo sleep esperando interrup��o 
       
		LED_Toggle(LED1_GPIO);	// quando � acordado atende a interrup��o, pisca uma vez e volta para sleep
		delay_ms(500);
		LED_Toggle(LED1_GPIO);
		delay_ms(500);
	}
	

}

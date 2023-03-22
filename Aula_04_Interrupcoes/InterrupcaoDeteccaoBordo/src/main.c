/* Gabarito - Interrupções PIO */
#include <asf.h>
#include "string.h" //obs: não necessário nesta aplicação

//protótipos das funções

void inicializacao_UART (void);
void tratamento_interrupcao_pioB(const uint32_t id, const uint32_t index);
void configurar_botao1 (void);


#define CONF_UART              UART0
#define CONF_UART_BAUDRATE     115200
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT
#define CONF_UART_PARITY       US_MR_PAR_NO
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT


#define BT3 PIO_PB3   // usa botão BP2 (USRPB1) ligado no pino 3 do PIOB 

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


void tratamento_interrupcao_pioB(const uint32_t id, const uint32_t index) // rotina de interrupção
{
	puts("\n\rEntrando - Interrupcao B\r");
	puts("*Parametros entrada\r");
	printf( "****id = %u \n\r", (unsigned int)id );		  // o id é a instância (periférico) que interrompeu que para o PIOB é 12
	printf( "****index = %u \n\r", (unsigned int)index ); // o index é o pino que interrompeu em decimal. No caso pino 3 dá 8 em decimal
	
	LED_Toggle(LED0_GPIO);						// esta é uma função de alto nível que vem no ambiente da Atmel pois carregamos um kit conhecido
												// LED0_GPIO é o pino 19 do PIOA --> Led azul
	
}


void configurar_botao1 (){
	
	pmc_enable_periph_clk(ID_PIOB); // habilita o clock do periférico PIOB (para usar debouncing)
	pio_set_input(PIOB, PIO_PB3, PIO_PULLUP | PIO_DEBOUNCE);  // configura pino 3 do PIOB como entrada (PIO_PB3 = 1<<3), ativa pull up e 
															  // deboncing (função API PIO). 
															  
	NVIC_DisableIRQ(PIOB_IRQn);		// desabilita a interrupção no NVIC do PIOB que tem ID =12 (instância id PIOB = PIOB_IRQn=12) 
															   
	// configura interrupção do PIOB (intância 12 = ID_PIOB), Pino 3 , (PIO_PB3),borda de subida (PIO_IT_RISE_EDGE). 
	//Quando isto ocorrer chama a rotina especificada. No caso atribuiu-se o nome: "tratamento_interrupcao_pioB()" 		
													  
	pio_handler_set(PIOB, ID_PIOB, PIO_PB3, PIO_IT_RISE_EDGE, tratamento_interrupcao_pioB);
	
	
	pio_enable_interrupt(PIOB, PIO_PB3); //habilita interrupção do pino 3 do PIOB no registrador PIO_IER do PIOB (função da API PIO)
	
	NVIC_ClearPendingIRQ(PIOB_IRQn); // limpa interrupções pendentes do PIOB
	NVIC_SetPriority(PIOB_IRQn,15);  // seta a prioridade da interrupção no controlador de interrupção (NVIC) como 15
	NVIC_EnableIRQ(PIOB_IRQn);       // habilita a interrupção no NVIC do PIOB
}



int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	
	inicializacao_UART();
	configurar_botao1();

	//Configura os PIOs de saida (LEDs)
	pio_set_output(PIOA, PIO_PA19, LOW, DISABLE, ENABLE); // função do PIO ASF: configura o pino 19 do PIOA como saída, nível baixo, saída não
														  // é dreno aberto, e ativa pull up no pino
	pio_set_output(PIOA, PIO_PA20, LOW, DISABLE, ENABLE); // idem para pino 20 do PIOA
	
	while (1)
	{
        /**
         * Entra em modo Sleep 
         * só é acordado por uma interrupção
         */
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI); //manda o processador para modo sleep esperando interrupção 
       
		LED_Toggle(LED1_GPIO);	// quando é acordado atende a interrupção, pisca uma vez e volta para sleep
		delay_ms(500);
		LED_Toggle(LED1_GPIO);
		delay_ms(500);
	}
	

}

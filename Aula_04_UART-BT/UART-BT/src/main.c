#include <asf.h>

/**
 *	UART 
 *
 *    (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *   - 9600 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 */
#define CONF_UART              UART0
#define CONF_UART_BAUDRATE     9600
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT
#define CONF_UART_PARITY       US_MR_PAR_NO
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT

#define interrupcao_UART     UART0_Handler   // apelido da rotina de interrupção de recepção da serial

#define LED_VERDE    IOPORT_CREATE_PIN(PIOA, 20)
#define LED_AZUL     IOPORT_CREATE_PIN(PIOA, 19)
#define MY_BUTTON    IOPORT_CREATE_PIN(PIOB, 3) //USRPB1
#define BOTAO_USRPB2 IOPORT_CREATE_PIN(PIOC, 12)//USRPB2


//protótipos das funçoes criadas

void configurar_botao1(void);
void tratamento_interrupcao_pioB (const uint32_t id, const uint32_t index);
void menu(void);
void inicializacao_UART (void);

volatile unsigned int key;			//tecla lida
volatile unsigned char nova_tecla;  //flag para indicar se existe nova tecla

void menu(){
		printf("\n\r");
		puts("#############-Menu-#########\r");
		puts("#  a: Acender Led Verde    #\r");
		puts("#  b: Acender Led Azul     #\r");
		puts("#  c: Apagar  Led Verde    #\r");
		puts("#  d: Apagar  Led Azul     #\r");
		puts("############################\r");
		printf("\n\r");
}

// rotina interrupção USRBP1
void tratamento_interrupcao_pioB (const uint32_t id, const uint32_t index){
	
	if ((id == ID_PIOB) && (index == PIO_PB3)){  //se tiver outras interrupções nos pinos de PIOB com mesma prioridade e quiser saber qual ativou
		
		if (ioport_get_pin_level(BOTAO_USRPB2)) //lê botão USRPB2 e checa se '1' ou '0'
			puts("USRPB2 nivel alto, nao pressionado\r");
		else
		  {
			puts("\r");
			puts("USRPB2 nivel baixo, PRESSIONADO\r");
		  }
	}

}

// tratamento interrupção UART

void interrupcao_UART(void){
	uint32_t status = usart_get_status((Usart *)UART0); //obtém status da interrupção da serial

	if (status & US_CSR_RXRDY) {  //isola para ver se a interrupção foi de caracter pronto para ler
		uint32_t caracter;
		usart_read((Usart *)UART0, &caracter);  // lê caracter (uint_32)
		key =(char)caracter;  //faz um casting para char
		printf("entrou na serial e teclou %c \n\r", key);
		nova_tecla = true;  //indica que tem nova tecla a tratar
	}
}
void configurar_botao1(){
	//pmc_enable_periph_clk(ID_PIOB);			//habilita clock PIOB MAS NÃO PRECISO POIS ESTÁ USANDO ioport_init() que faz isso
	
	//Podemos configurar as entradas usando as funções do driver PIO como abaixo ou do IOPORT:
	//pio_set_input(PIOB, PIO_PB3, PIO_PULLUP | PIO_DEBOUNCE ); //debouncing e pullup no PB3, botão PB2
	//pio_set_input(PIOC, PIO_PC12, PIO_PULLUP);
	//-------------------------------------
	//No lugar de pio_set_input() do PIO
	// podemos usar também as funções do IOPORT para programar a entrada:
	ioport_set_pin_dir(MY_BUTTON, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(MY_BUTTON, IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE);
	ioport_set_pin_dir(BOTAO_USRPB2, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(BOTAO_USRPB2, IOPORT_MODE_PULLUP);
	//----------------------------------------------------
	
	NVIC_DisableIRQ(PIOB_IRQn);                //desabilita interrupção instância 12 = PIOB_IRQn,ou seja, do PIOB
	 
	// associa rotina de inetrrupção "tratamento_interrupcao_pioB" ao PIOB, ID_PIOB=12, pino 3 = PIO_PB3, borda de descida
	
	pio_handler_set(PIOB, ID_PIOB, PIO_PB3, PIO_IT_FALL_EDGE, tratamento_interrupcao_pioB);
	pio_enable_interrupt(PIOB, PIO_PB3); //habilita interruppção Id 12 = PIOB
	NVIC_ClearPendingIRQ(PIOB_IRQn);	//limpa eventual interruppção pendente Id 12 = PIOB
	NVIC_SetPriority (PIOB_IRQn,15);	//seta prioridade da interrupção Id 12 como 15 (mais baixa)
	NVIC_EnableIRQ(PIOB_IRQn);			//habilita interrupção 12
}


void inicializacao_UART (){
	
	 static usart_serial_options_t usart_options = {
		 .baudrate = CONF_UART_BAUDRATE,
		 .charlength = CONF_UART_CHAR_LENGTH,
		 .paritytype = CONF_UART_PARITY,
		 .stopbits = CONF_UART_STOP_BITS
	 };
	
	usart_serial_init((Usart *)CONF_UART, &usart_options);
	
	stdio_serial_init((Usart *)CONF_UART, &usart_options);
	
	NVIC_DisableIRQ(UART0_IRQn);   // desabilita interrupção. Id da UART = 8
	usart_enable_interrupt((Usart *)UART0, US_IER_RXRDY);  // habilita interrupção por RXready (recepção caracter) 
	NVIC_ClearPendingIRQ(UART0_IRQn); // limpa interrupção pendente UART
	NVIC_SetPriority (UART0_IRQn,13); // prioridade 13 para a interrupção da UART. Mais prioritária que do botão
	NVIC_EnableIRQ(UART0_IRQn);   // Id da UART = 8
}


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	
	inicializacao_UART();
	ioport_init();
	
	configurar_botao1();
	
	ioport_set_pin_dir(LED_VERDE, IOPORT_DIR_OUTPUT); //leds verdes e azuis como saídas
	ioport_set_pin_dir(LED_AZUL, IOPORT_DIR_OUTPUT);
	
	menu();	
	
	while(1){

		if (nova_tecla)    // verifica se tem nova tecla a tratar
		{
			nova_tecla=false; // se tiver indica que já leu e trata a tecla
				
			switch (key) {
				case 'a':
					ioport_set_pin_level(LED_VERDE, 0); //acende
				break;
				case 'b':
					ioport_set_pin_level(LED_AZUL, 0);
				break;
				case 'c':
					ioport_set_pin_level(LED_VERDE, 1); //apaga
				break;
				case 'd':
					ioport_set_pin_level(LED_AZUL, 1);
				break;
				case 'm':
					menu();	
				break;					
				default:
					printf("Opcao nao definida: %c \n\r", key);
			}
	   }
		
	}



}
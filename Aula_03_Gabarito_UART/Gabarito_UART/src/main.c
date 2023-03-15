/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "string.h"

void inicializacao_UART (void);  //protótipo da função

#define CONF_UART              UART0				//usa UART 0
#define CONF_UART_BAUDRATE     9600				//9600 bps
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT		// 8 bits
#define CONF_UART_PARITY       US_MR_PAR_NO			//sem paridade
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT	// 1 stop bit

#define LED_VERDE    IOPORT_CREATE_PIN(PIOA, 20)  //LED1
#define LED_AZUL     IOPORT_CREATE_PIN(PIOA, 19)  //LED0


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


int main (void)
{
	unsigned char key;
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	ioport_init();				 //inicializa o serviço IOPORT
	ioport_enable_pin(LED_VERDE) ; // habilita led verde no PIO
	ioport_set_pin_dir(LED_VERDE,IOPORT_DIR_OUTPUT); // LED como saída
	ioport_enable_pin(LED_AZUL) ; // habilita led verde no PIO
	ioport_set_pin_dir(LED_AZUL,IOPORT_DIR_OUTPUT); // LED como saída
	
	
	inicializacao_UART();
	
	puts("1: Exibir menu novamente.\n\r 2: Ativar o LED AZUL \n\r 3: Desligar o LED AZUL \n\r 4: Ativar o LED VERDE \n\r 5: Desligar o LED VERDE \n\r");
	
	while(1){
		//usart_serial_getchar(CONF_UART, &key);
		key = getchar();
		switch (key) {
			case '1':
				puts("1: Exibir menu novamente.\n\r 2: Ativar o LED AZUL \n\r 3: Desligar o LED AZUL \n\r 4: Ativar o LED VERDE \n\r 5: Desligar o LED VERDE \n\r");
			break;
			case '2':
				ioport_set_pin_level(LED_AZUL, 0); //acende led azul
			break;
			case '3':
				ioport_set_pin_level(LED_AZUL, 1); // apaga led azul
			break;
			case  '4':
				ioport_set_pin_level(LED_VERDE, 0);// acende led verde
			break;
			case '5':
				ioport_set_pin_level(LED_VERDE, 1);
			break;
			default:
				puts("Opção Invalida!\n\r");
				puts("\n\r");
				puts("1: Exibir menu novamente.\n\r 2: Ativar o LED AZUL \n\r 3: Desligar o LED AZUL \n\r 4: Ativar o LED VERDE \n\r 5: Desligar o LED VERDE \n\r");
		}
	}
}

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
#include <stdio.h>
#define USART_SERIAL                     UART0
#define USART_SERIAL_BAUDRATE            115200
#define USART_SERIAL_CHAR_LENGTH         US_MR_CHRL_8_BIT
#define USART_SERIAL_PARITY              US_MR_PAR_NO
#define USART_SERIAL_STOP_BIT            US_MR_NBSTOP_1_BIT
void inicializacao_UART(void);

void inicializacao_UART(){
	static usart_serial_options_t usart_options = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	usart_serial_init(USART_SERIAL, &usart_options);
	stdio_serial_init(USART_SERIAL, &usart_options);
}

int main (void){
	/* Insert system clock initialization code here (sysclk_init()). */
	unsigned char received_byte;
	sysclk_init();
	board_init();
	/* Insert application code here, after the board has been initialized. */
	inicializacao_UART();
	puts("Ola serial!\r");
	while(1){
		//usart_serial_getchar((Usart*)USART_SERIAL, &received_byte);
		//usart_serial_putchar((Usart*)USART_SERIAL, received_byte);
		received_byte = getchar();
		switch(received_byte){
			case '1': 
				printf("Pressionou: %d, %c\n\r", received_byte,received_byte);
				printf("Valor OK\n\r");
				break;
			case '2':
				printf("Pressionou: 2\n\r");
				break;
			default:
				printf("opcao errada!\n\r");
		}
	}


}
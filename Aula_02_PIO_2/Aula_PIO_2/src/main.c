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
//#define LED_VERDE (1<<20)
//#define BP2 (1<<3)
#define LED_VERDE IOPORT_CREATE_PIN(PIOA,20)
#define BP2 IOPORT_CREATE_PIN(PIOB,3)

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	ioport_init();
	// Usando programação direto com as portas.
	//PIOA->PIO_PER |= Green;
	//PIOA->PIO_OER |= Green;
	//PIOB->PIO_PER |= BP2;
	//PIOB->PIO_ODR |= BP2;
	//PIOB->PIO_PUER |= BP2;
	//while(1){
		//if(PIOB->PIO_PDSR & BP2){
			//PIOA->PIO_SODR |= Green;
		//}
		//else PIOA->PIO_CODR |= Green;
	//}
	// Usando as ferramentas de IO direto da placa.
	ioport_enable_pin(LED_VERDE);
	ioport_enable_pin(BP2);
	ioport_set_pin_dir(LED_VERDE, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(BP2,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(BP2,IOPORT_MODE_PULLUP);
	while(1){
		if(ioport_get_pin_level(BP2)) ioport_set_pin_level(LED_VERDE,1);
		else ioport_set_pin_level(LED_VERDE,0);
	}
	/* Insert application code here, after the board has been initialized. */
}

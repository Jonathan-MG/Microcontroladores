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
#include <delay.h>
#define Blue (1<<19)
#define Green (1<<20)

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	
	/* Código de funcionamento na raça!
	uint32_t *RA_PER = (uint32_t*) 0x400E0E00;
	uint32_t *RA_OER = (uint32_t*) 0x400E0E10;
	uint32_t *RA_SODR = (uint32_t*) 0x400E0E30;
	uint32_t *RA_CODR = (uint32_t*) 0x400E0E34;
	
	int Blue = (1<<19);
	int Green = (1<<20); //0x100000
	int Both_LED = (11<<19); //0x180000 - Linha desnecessária por conta do OR
	
	*RA_PER = Both_LED;
	*RA_OER = Both_LED;
	*RA_CODR = Both_LED; // Liga o LED
	*RA_SODR = Both_LED; // Desliga o LED
	*/
	
	PIOA->PIO_PER |= Blue | Green; // Fazer '|' 'OU', assim os bits nos outros pinos não serão alterados.
	PIOA->PIO_OER |= Blue | Green;
	
	while(1){
		PIOA->PIO_CODR = Blue;
		PIOA->PIO_SODR = Green;
		delay_ms(500);
		PIOA->PIO_CODR = Green;
		PIOA->PIO_SODR = Blue;
		delay_ms(500);
	}

	/* Insert application code here, after the board has been initialized. */
}

/*
 Controla a luminosidade do led Azul atrav�s dos bot�es usando PWM de 20 kHz
 USERPB1 (BP2) aumenta a luminosidade
 USERPB2 (BP3) diminui a liminosidade
 Note que o Led acende em zero, ent�o qto mais o pino est� em n�vel zero maior � o brilho
*/
#include "asf.h"

// Prot�tipos fun��es

void aumenta(const uint32_t id, const uint32_t index);	//aumenta luminosidade
void diminui(const uint32_t id, const uint32_t index);	//diminui luminosidade
void configurar_botao1(void);							//configura BP2
void configurar_botao2(void);							//configura BP3
static void configurar_pwm(void);						// configura PWM para pino 19 (led azul)

#define PIN_BUTTON		3		//bot�o BP2 (USRPB1)
#define PIN_BUTTON3     12		//bot�o BP3 (USRPB2)

// define como pinos de sa�da do PWM o pinos PA19 (led azul)
// Ver tabela 39.2 do datasheet para os pinos de sa�da poss�veis
// Notar que PA19 utiliza PWML0 (PWM lado low do canal 0) 
//----------------------------------------------
#define LED_AZUL IOPORT_CREATE_PIN(PIOA,19)
//----------------------------------------------

/** 
 * Defini��o dos ports
 * referentes aos bot�es
 */
#define PORT_BUT_2		PIOB	//BP2
#define PORT_BUT_3		PIOC	//BP3

/**
 * Define os IDs dos perif�ricos associados aos pinos
 */
#define ID_BUT_2		ID_PIOB	//BP2 no PB3
#define ID_BUT_3		ID_PIOC	//BP3 no PC12

/**
 *	Define as masks utilIzadas
 */
#define MASK_BUT_2		(1u << PIN_BUTTON)	//BP2 no PB3
#define MASK_BUT_3		(1U << PIN_BUTTON3)	//BP3 no PC12

/** PWM frequency in Hz */
#define PWM_FREQUENCY      20000			//frequ�ncia desejada do PWM em Hz (20 kHz -> T= 50us)

/** Period value of PWM output waveform */
/*
per�odo do PWM desejado � PERIOD_VALUE (=100) vezes o
n�mero de ciclos de clock programado no gerador de clock do PWM (ul_clka)
ou seja: este valor dividi a frequ�ncia do clock programado para o PWM por 100
Ver estrutura clock_setting abaixo.
*/
#define PERIOD_VALUE       100

/** Initial duty cycle value */
#define INIT_DUTY_VALUE    0
		
/** PWM channel instance for LEDs */
pwm_channel_t g_pwm_channel_led;			// cria estrutura do tipo pwm_channel_t para configurar PWM
volatile uint32_t ul_duty = INIT_DUTY_VALUE;  /* PWM duty cycle rate */


//rotina de interrup��o BP2 (USRPB1) aumenta o duty. No caso aumenta o n�vel low da sa�da PWM_L0 (led com mais brilho)
void aumenta(const uint32_t id, const uint32_t index) {
	/* Set new duty cycle */
	ul_duty = ul_duty + 10;
	if(ul_duty>PERIOD_VALUE) ul_duty = PERIOD_VALUE; // duty n�o pode ser maior que o per�odo
	g_pwm_channel_led.channel = PIN_PWM_LED0_CHANNEL;
	pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_duty);
}

//rotina de interrup��o BP3 (USRPB2) aumenta o duty. No caso diminui o n�vel low da sa�da PWM_L0 (led com menos brilho)
void diminui(const uint32_t id, const uint32_t index) {
	/* Set new duty cycle */
	ul_duty = ul_duty - 10;
	if(ul_duty > PERIOD_VALUE) ul_duty = 0; // como ul_duty � unsigned se der menor que zero estoura e d� maior que
											// o PERIOD_VALUE
	g_pwm_channel_led.channel = PIN_PWM_LED0_CHANNEL;
	pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_duty);
}

void configurar_botao1 (){
	pio_set_input(PORT_BUT_2, MASK_BUT_2, PIO_PULLUP | PIO_DEBOUNCE);
	/* 
	*	Configura interrup��o para acontecer em borda de subida.
	*/
	pio_handler_set(PORT_BUT_2, //endere�o do PIOB
					ID_BUT_2,   //ID do PIOB
					MASK_BUT_2,  //M�scara do bot�o
					PIO_IT_RISE_EDGE, //Habilita como borda de subida
					aumenta); //informa a rotina de interrup��o
	
	/*
	*	Ativa interrup��o no perif�rico B porta do bot�o
	*/	
	pio_enable_interrupt(PORT_BUT_2, MASK_BUT_2); //Habilita o bot�o como interrup��o
	
	/*
	*	Configura a prioridade da interrup��o no pORTB
	*/
	
	NVIC_SetPriority(PIOB_IRQn,10); //Configurando prioridade do m�dulo PIOB no NVIC
	/*
	*	Ativa interrup��o no port B
	*/
	NVIC_EnableIRQ(PIOB_IRQn);  //Habilita o PIOB no NVIC
	
}

void configurar_botao2 (){
	pio_set_input(PORT_BUT_3, MASK_BUT_3, PIO_PULLUP | PIO_DEBOUNCE);
	/* 
	*	Configura interrup��o para acontecer em borda de subida.
	*/
	pio_handler_set(PORT_BUT_3, //endere�o do PIOC
					ID_BUT_3,   //ID do PIOC
					MASK_BUT_3,  //M�scara do bot�o
					PIO_IT_RISE_EDGE, //Habilita como borda de subida
					diminui); //informa a rotina de interrup��o
	
	/*
	*	Ativa interrup��o no perif�rico C porta do bot�o
	*/	
	pio_enable_interrupt(PORT_BUT_3, MASK_BUT_3); //Habilita o bot�o como interrup��o
	
	/*
	*	Configura a prioridade da interrup��o no PORTC
	*/
	
	NVIC_SetPriority(PIOC_IRQn,11); //Configurando prioridade do m�dulo PIOC no NVIC
	/*
	*	Ativa interrup��o no port C
	*/
	NVIC_EnableIRQ(PIOC_IRQn);  //Habilita o PIOC no NVIC
	
}

static void configurar_pwm(void)
{
	
	// Configura os pinos de I/O usados para o PWM
	// ver tabela 39.2 no datasheet dos SAM4S
	//-------------------------------------------------------
	// usando PIO
	pio_configure_pin(LED_AZUL, PIO_TYPE_PIO_PERIPH_B);
	
	// outra op��o � usando o servi�o IOPORT em vez do driver PIO para programar os pinos p/PWM
	
	//ioport_init();
	//ioport_set_pin_mode(LED_AZUL,IOPORT_MODE_MUX_B);
	
	//---------------------------------------------------------
	
	/* Enable PWM peripheral clock */
	pmc_enable_periph_clk(ID_PWM);		//ID_PWM = 31

	/* Disable PWM channels for LEDs */
	pwm_channel_disable(PWM, PIN_PWM_LED0_CHANNEL);	// desabilita canal 0 do PWM

	/* Set PWM clock A as PWM_FREQUENCY*PERIOD_VALUE (clock B is not used) */
	/* ul_clka � o valor do clock A em Hz (Fig. 39.3 datasheet. Este valor vai ser dividido pelo PERIOD_VALUE, valor programado
	 no registro de per�odo do PWM. Programando ul.clka = PWM_FREQUENCY * PERIOD_VALUE, quando este valor for dividido
	 pelo registro de per�odo no HW do PWM, obtemos a frequ�ncia PWM_FREQUENCY desejada.
	*/
	pwm_clock_t clock_setting = {
		.ul_clka = PWM_FREQUENCY * PERIOD_VALUE,	//frequ�ncia clock A em Hz. Este valor vai ser dividido pelo PERIOD_VALUE
		.ul_clkb = 0,								// desliga clock B
		.ul_mck = sysclk_get_cpu_hz()				// mck � o master clock da placa
	};
	
	pwm_init(PWM, &clock_setting);					//aplica as configura��es de clock

	/* Initialize PWM channel for LED0 */
	/* Period is left-aligned */
	/*PWM com alinhamento � esquerda (ver pag. 960 e 961 do datsheet)
	mudando do modo alinhado � esquerda para mode alinhado no centro ir� causar o per�odo do PWM e a largura do estado ativo 
	do pulso PWM aumentar por um fator de 2 (dobra os tempos).
	*/
	g_pwm_channel_led.alignment = PWM_ALIGN_LEFT;
	
	/* Output waveform starts at a low level */
	/*Se este valor � setado para PWM_HIGH, o valor �ul_duty� define a largura do n�vel l�gico alto do signal 
	(em outras palavras, n�vel alto � o estado ativo); se setado para  PWM_LOW, o valor de  �ul_duty� define a largura da
	por��o do n�vel l�gico baixo do sinal. A refer�ncia � o sinal PWM_Hx, x o n�mero do canal. Se PWM_Hx = alto
	implica que PWM_Lx � baixo (inverso). De maneira equivalente: polarity = PWM_HIGH --> duty_controla o n�vel alto do sinal
	PWM_Hx; polarity = PWM_LOW --> duty controla o n�vel alto do sinal PWM_Lx.
	*/
	g_pwm_channel_led.polarity = PWM_HIGH;				// define n�vel ativo do PWM como alto (para Sa�da PWM_Hx)
	/* Use PWM clock A as source clock */
	g_pwm_channel_led.ul_prescaler = PWM_CMR_CPRE_CLKA; // programa como fonte de clock para o canal PWM o clock A
	/* Period value of output waveform */
	/* per�odo do PWM desejado � PERIOD_VALUE (=100) vezes o  n�mero de ciclos de clock programado no gerador de clock do PWM 
	  (ul_clka),ou seja, este valor dividi a frequ�ncia do clock programado para o PWM por PERIOD_VALUE
	*/
	g_pwm_channel_led.ul_period = PERIOD_VALUE;
	
	/* Duty cycle value of output waveform */
	/* Duty cycle value of output waveform */
	/* o duty cycle � a dura��o do pulso do PWM em ticks de clock do canal A (no caso). Se polaridade do PWM � High � o tempo 
	que fica em n�vel alto, n�o � o valor da percentagem direto. Se o valor de INIT_DUTY_VALUE for 50, o duty ser� 
	50/PERIOD_VALUE = 50/100 = 50% (para polaridade PWM_HIGH).No caso, como definimos PERIOD_VALUE como 100, os valores 
	coincidem.
	 */
	g_pwm_channel_led.ul_duty = INIT_DUTY_VALUE;		//ul_duty inicial
	
	g_pwm_channel_led.channel = PIN_PWM_LED0_CHANNEL;	// Vamos usar o canal 0 do PWM
	pwm_channel_init(PWM, &g_pwm_channel_led);			// aplica as configura��es iniciando o PWM canal 0

	/* Enable PWM channels for LEDs */
	pwm_channel_enable(PWM, PIN_PWM_LED0_CHANNEL);		//habilita PWM
	
}


/**
 * \brief Application entry point for PWM with LED example.
 * Output PWM waves on LEDs to make them fade in and out.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Configure the console uart for debug information */
	
	configurar_botao1();	//BP2 (USRPB1)
	configurar_botao2();	//BP3 (USRPB2)
	configurar_pwm();		// configura PWM
		
	/* Infinite loop */
	while (1) {
	}
}
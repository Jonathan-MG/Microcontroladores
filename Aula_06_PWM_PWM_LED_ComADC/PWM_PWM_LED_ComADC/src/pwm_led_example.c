/* Varia o duty cycle de um PWM de 20 Khz, acionando os led azul, através do 
potenciômetro do kit. O led verde fica fixo com um PWM de 50% de duty_cycle */
  
#include "asf.h"

//PROTÓTIPOS
static void configure_pwm(void);	//configura PWM
static void configure_adc(void);	//configura conversor AD
static void configure_tc(void);	    //configura PWM

/*OBs:
 TC0_Handler --> int timer 0 canal 0 (ID_TC0)
 TC1_Handler --> int timer 0 canal 1 (ID_TC1)
 TC2_Handler --> int timer 0 canal 2 (ID_TC2)
 TC3_Handler --> int timer 1 canal 0 (ID_TC3)
 TC4_Handler --> int timer 1 canal 1 (ID_TC4)
 TC3_Handler --> int timer 1 canal 2 (ID_TC5)
 */

// define como pinos de saída do PWM os pinos PA19 (led azul) e PA20 (led verde)
// Ver tabela 39.2 do datasheet para os pinos de saída possíveis 
// Notar que PA19 utiliza PWML0 (PWM lado low do canal 0) e PA20 usaa PWML1 (PWM lado low canal 1) 
//----------------------------------------------
#define LED_VERDE IOPORT_CREATE_PIN(PIOA,20)
#define LED_AZUL IOPORT_CREATE_PIN(PIOA,19)
//----------------------------------------------

#define TC			TC1      //usa timer_counter 1
#define CHANNEL		0		 //canal 0 
#define ID_TC		ID_TC3   // ID do canal 0 do TC1
#define TC_IRQn     TC3_IRQn // Interrupção canal 0 do TC1

#define IRQ_PRIOR_ADC    0	// prioridade interrupção do ADC

#define ADC_CLOCK   6400000

/** PWM frequency in Hz */
#define PWM_FREQUENCY      20000			//frequência desejada do PWM em Hz (20 kHz -> T= 50us)
/** Period value of PWM output waveform */
#define PERIOD_VALUE       100				// período do PWM desejado é PERIOD_VALUE (=100) vezes o
											// número de ciclos de clock programado no gerador de clock do PWM (ul_clka)
											// ou seja: este valor dividi a frequência do clock programado para o PWM por 100
											// Ver estrutura clock_setting abaixo
/** Initial duty cycle value */
#define INIT_DUTY_VALUE    0

pwm_channel_t g_pwm_channel_led; // cria estrutura do tipo pwm_channel_t para configurar PWM


// rotina interrupção ADC (lê canal 5 - Potenciômetro)
void ADC_Handler(void)
{
	if ((adc_get_status(ADC) & ADC_IER_EOC5) == ADC_IER_EOC5) {
		uint32_t result = adc_get_latest_value(ADC);				
		double ul_duty =  (result*PERIOD_VALUE)/4096;		//duty cycle pode ir no máximo até o valor do PERIOD_VALUE (100)
															// ADC é de 0 a 4095
		g_pwm_channel_led.channel = PIN_PWM_LED0_CHANNEL;
		pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_duty); // muda o duty do canal 0 (led azul)		
																   // led verde mantém como iniciado
	}
}


static void configure_adc(void)  // configura ADC
{
	pmc_enable_periph_clk(ID_ADC);
	adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 8);
	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);
	adc_enable_channel(ADC, ADC_CHANNEL_5);
	adc_enable_interrupt(ADC, ADC_IER_EOC5); //habilita interrupção quando tiver completado a conversão do canal 5;
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);

	NVIC_SetPriority(ADC_IRQn, IRQ_PRIOR_ADC);
	NVIC_EnableIRQ(ADC_IRQn);
	adc_start(ADC);
}

// rotina interrupção timer 1 canal 0 - Dispara a cada 200 Hz
void TC3_Handler(void) 
{
	tc_get_status(TC,CHANNEL);
	adc_start(ADC);				//dispara timer

}

static void configure_tc(void)  // configura timer 1 canal 0
{
	uint32_t freq_desired = 200;	//timer 1 canal 0 frequência 200 Hz
	uint32_t ul_tcclk;
	uint32_t ul_div;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	pmc_enable_periph_clk(ID_TC);
	tc_find_mck_divisor(freq_desired,ul_sysclk,&ul_div,&ul_tcclk,ul_sysclk);
	tc_init(TC,CHANNEL,TC_CMR_CPCTRG|ul_tcclk);

	uint32_t counter = (ul_sysclk/ul_div)/freq_desired;
    tc_write_rc(TC,CHANNEL,counter);
	tc_enable_interrupt(TC,CHANNEL,TC_IER_CPCS);
	NVIC_EnableIRQ(TC_IRQn);
    tc_start(TC,CHANNEL);
}

static void configure_pwm(void)
{
	
	// Configura os pinos de I/O usados para o PWM
	// ver tabela 39.2 no datasheet dos SAM4S
	//-------------------------------------------------------
	// usando PIO
	pio_configure_pin(LED_AZUL, PIO_TYPE_PIO_PERIPH_B);
	pio_configure_pin(LED_VERDE,PIO_TYPE_PIO_PERIPH_B);
	
	// outra opção é usando o serviço IOPORT em vez do driver PIO para programar os pinos p/PWM
	//ioport_set_pin_mode(LED_AZUL,IOPORT_MODE_MUX_B);
	//ioport_set_pin_mode(LED_VERDE,IOPORT_MODE_MUX_B);
	//---------------------------------------------------------
	
	/* Enable PWM peripheral clock */
	pmc_enable_periph_clk(ID_PWM);    //ID_PWM = 31

	/* Disable PWM channels for LEDs */
	pwm_channel_disable(PWM, PIN_PWM_LED0_CHANNEL); // desabilita canal 0 do PWM
	pwm_channel_disable(PWM, PIN_PWM_LED1_CHANNEL); // desabilita canal 1 do pwm


	/* Set PWM clock A as PWM_FREQUENCY*PERIOD_VALUE (clock B is not used) */
	/* ul_clka é o valor do clock A em Hz (Fig. 39.3 datasheet. Este valor vai ser dividido pelo PERIOD_VALUE, valor programado
	 no registro de período do PWM. Programando ul.clka = PWM_FREQUENCY * PERIOD_VALUE, quando este valor for dividido
	 pelo registro de período no HW do PWM, obtemos a frequência PWM_FREQUENCY desejada.
	*/
	pwm_clock_t clock_setting = {
		.ul_clka = PWM_FREQUENCY * PERIOD_VALUE,     //frequência do clock A em Hz. Este valor vai ser dividido pelo  
		.ul_clkb = 0,							     // desliga clock B
		.ul_mck = sysclk_get_cpu_hz()				 // mck é o master clock da placa
	};

	pwm_init(PWM, &clock_setting);					//aplica as configurações de clock

	/* Initialize PWM channel for LED0 (LED AZUL) */
	/* Period is left-aligned */
	/*PWM com alinhamento à esquerda (ver pag. 960 e 961 do datsheet)
	mudando do modo alinhado à esquerda para mode alinhado no centro irá causar o período do PWM e a largura do estado ativo 
	do pulso PWM aumentar por um fator de 2 (dobra os tempos).
	*/
	g_pwm_channel_led.alignment = PWM_ALIGN_LEFT;
	
	/* Output waveform starts at a low level */
	/*Se este valor é setado para PWM_HIGH, o valor “ul_duty” define a largura do nível lógico alto do signal 
	(em outras palavras, nível alto é o estado ativo); se setado para  PWM_LOW, o valor de  “ul_duty” define a largura da
	porção do nível lógico baixo do sinal. A referência é o sinal PWM_Hx, x o número do canal. Se PWM_Hx = alto
	implica que PWM_Lx é baixo (inverso). De maneira equivalente: polarity = PWM_HIGH --> duty_controla o nível alto do sinal
	PWM_Hx; polarity = PWM_LOW --> duty controla o nível alto do sinal PWM_Lx.
	*/
	g_pwm_channel_led.polarity = PWM_LOW;				// define nível ativo do PWM como baixo (para Saída PWM_Hx)   
	/* Use PWM clock A as source clock */
	g_pwm_channel_led.ul_prescaler = PWM_CMR_CPRE_CLKA; // programa como fonte de clock para o canal PWM o clock A
	
	/* Period value of output waveform */
	/* período do PWM desejado é PERIOD_VALUE (=100) vezes o  número de ciclos de clock programado no gerador de clock do PWM 
	  (ul_clka),ou seja, este valor dividi a frequência do clock programado para o PWM por PERIOD_VALUE
	*/
	g_pwm_channel_led.ul_period = PERIOD_VALUE;
	
	/* Duty cycle value of output waveform */
	/* o duty cycle é a duração do pulso do PWM em ticks de clock do canal A (no caso). Se polaridade do PWM é High é o tempo 
	que fica em nível alto, não é o valor da percentagem direto. Se o valor de INIT_DUTY_VALUE for 50, o duty será 
	50/PERIOD_VALUE = 50/100 = 50% (para polaridade PWM_HIGH).No caso, como definimos PERIOD_VALUE como 100, os valores 
	coincidem.
	 */
	g_pwm_channel_led.ul_duty = INIT_DUTY_VALUE;
	
	g_pwm_channel_led.channel = PIN_PWM_LED0_CHANNEL; // Vamos usar o canal 0 do PWM
	pwm_channel_init(PWM, &g_pwm_channel_led);	// aplica as configurações iniciando o PWM canal 0

	/* Enable channel counter event interrupt */
	//pwm_channel_enable_interrupt(PWM, PIN_PWM_LED0_CHANNEL, 0);

	/* Initialize PWM channel for LED1 (REPETE PARA LED VERDE)*/
	/* Period is center-aligned */
	g_pwm_channel_led.alignment = PWM_ALIGN_CENTER;
	/* Output waveform starts at a high level */
	g_pwm_channel_led.polarity = PWM_HIGH;
	/* Use PWM clock A as source clock */
	g_pwm_channel_led.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_led.ul_period = PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_led.ul_duty = INIT_DUTY_VALUE + 50; // coloca duty de 50% para os valores utilizados (50/100)
	g_pwm_channel_led.channel = PIN_PWM_LED1_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_led);

	//Exemplo de de alguns comandos caso se utilize interrupção PWM
	//------------------------------------------
	/* Disable channel counter event interrupt */
	//pwm_channel_disable_interrupt(PWM, PIN_PWM_LED1_CHANNEL, 0);

	/* Configure interrupt and enable PWM interrupt */
	//NVIC_DisableIRQ(PWM_IRQn);
	//NVIC_ClearPendingIRQ(PWM_IRQn);
	//NVIC_SetPriority(PWM_IRQn, 0);
	//NVIC_EnableIRQ(PWM_IRQn);
	//------------------------------------------
	
	/* Enable PWM channels for LEDs */
	pwm_channel_enable(PWM, PIN_PWM_LED0_CHANNEL);	//habilita PWM LED AZUL
	pwm_channel_enable(PWM, PIN_PWM_LED1_CHANNEL);	//habilita PWM LED VERDE
}


int main(void)
{
	sysclk_init();
	board_init();	

	configure_pwm();	//configura PWM
	configure_adc();	//configura conversor AD canal 5
	configure_tc();		//configura Timer 1 canal 0

	/* Infinite loop */
	while (1) {
	}
}
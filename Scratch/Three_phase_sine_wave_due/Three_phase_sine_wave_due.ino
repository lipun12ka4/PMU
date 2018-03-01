
//################################################################################################################
// **INCLUDE FILES & HEADER DATA****INCLUDE FILES & HEADER DATA****INCLUDE FILES & HEADER DATA**
//################################################################################################################

#include "asf.h" // ATMEL SOFTWARE  FRAMEWORK
#include "conf_board.h" 
#include "conf_clock.h"
#include "SineArray.h"

//----------------------------------------------------------------------------------------------------------------

//################################################################################################################
// **Global variables and macros****Global variables and macros****Global variables and macros**
//################################################################################################################

/** PWM frequency in Hz */
#define PWM_FREQUENCY     10000
/** Period value of PWM output waveform */
#define PERIOD_VALUE       4096
/** Initial duty cycle value */
#define INIT_DUTY_VALUE   50

//----------------------------------------------------------------------------------------------------------------

//################################################################################################################
// **GPIO pin definitions for 3 PWM signals****GPIO pin definitions for 3 PWM signals**
//##############################################################################################
/* PIN Definition for the third PWM Channel @ Arduino Due Board = PWM  PIN 07 */
#define PIN_PWM_3DPWM_GPIO     PIO_PC23_IDX
#define PIN_PWM_3DPWM_FLAGS   (PIO_PERIPH_B | PIO_DEFAULT)
#define PIN_PWM_3DPWM_CHANNEL PWM_CHANNEL_6


/* PIN Definition for the second PWM Channel @ Arduino Due Board = PWM  PIN 08 */
#define PIN_PWM_2NDPWM_GPIO     PIO_PC22_IDX
#define PIN_PWM_2NDPWM_FLAGS   (PIO_PERIPH_B | PIO_DEFAULT)
#define PIN_PWM_2NDPWM_CHANNEL PWM_CHANNEL_5	


/* PIN Definition for the first PWM Channel @ Arduino Due Board = PWM  PIN 09 */
#define PIN_PWM_1STPWM_GPIO     PIO_PC21_IDX
#define PIN_PWM_1STPWM_FLAGS   (PIO_PERIPH_B | PIO_DEFAULT)
#define PIN_PWM_1STPWM_CHANNEL PWM_CHANNEL_4

//----------------------------------------------------------------------------------------------------------------

/** PWM channel instance  */
pwm_channel_t g_pwm_channel_instance;

//################################################################################################################
// **Interrupt handler for the PWM controller****Interrupt handler for the PWM controller**
//################################################################################################################


void PWM_Handler(void)
{
	static uint32_t size;						 /* Size of the sine array*/
	static uint32_t ul_count = 0;				 /* PWM counter value */
	static uint32_t ul_duty1 = INIT_DUTY_VALUE;  /* PWM duty cycle rate */
	static uint32_t ul_duty2 = INIT_DUTY_VALUE;  /* PWM duty cycle rate */
	static uint32_t ul_duty3 = INIT_DUTY_VALUE;  /* PWM duty cycle rate */
	
	uint32_t events = pwm_channel_get_interrupt_status(PWM);

			 /* Standard frequency range e.g LENZE VFD 0-300 HZ */
			 /* Ground sine wave frequency = (PWM_Freq/ Size of sine wave array)
			  = 10 000 HZ/ 120 = 83.3 Hz*/
			  ul_count++;
			/* to vary the sine wave frequency the  u_count must be re defined due to the desired frequency*/
		//ul_count= ul_count+2 ;
		/* to determine the size of the sine array*/
		size = sizeof(sine)/2; 
				if (ul_count >= size) {
					ul_count = 0;
				}
				/* Overlay a sine wave array to a  PWM signal */
				/* each output signal is 120 degree phase shifted to the other one */
				ul_duty1 = sine[ul_count%size];//  // 1st sine wave output 0 degree phase shift
				ul_duty2 = sine[(ul_count+(size/3)) %size]; // 2nd sine wave output 120 degree phase shift
				ul_duty3 = sine[(ul_count+(2*size/3))%size];// 3rd sine wave output 240  degree phase shift 
			/* Set new duty cycle */
				g_pwm_channel_instance.channel = PIN_PWM_1STPWM_CHANNEL;
				pwm_channel_update_duty(PWM, &g_pwm_channel_instance, ul_duty1);
				g_pwm_channel_instance.channel = PIN_PWM_2NDPWM_CHANNEL;
				pwm_channel_update_duty(PWM, &g_pwm_channel_instance, ul_duty2);
				g_pwm_channel_instance.channel = PIN_PWM_3DPWM_CHANNEL;
				pwm_channel_update_duty(PWM, &g_pwm_channel_instance, ul_duty3);
			
		}
//########################################################################################################
// ******MAIN MENU*************************MAIN MENU*******************************MAIN MENU***************
//##############################################################################################
int main(void)
{
	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* GPIO Pin configuration */
	gpio_configure_pin(PIN_PWM_3DPWM_GPIO, PIN_PWM_3DPWM_FLAGS);  // for PWM PIN 3
	gpio_configure_pin(PIN_PWM_2NDPWM_GPIO, PIN_PWM_2NDPWM_FLAGS);  // for PWM PIN 2
	gpio_configure_pin(PIN_PWM_1STPWM_GPIO, PIN_PWM_1STPWM_FLAGS);  // for PWM PIN 1
	
	/* Enable PWM peripheral clock */
	pmc_enable_periph_clk(ID_PWM);

	/* Disable PWM channels */
	pwm_channel_disable(PWM, PIN_PWM_1STPWM_CHANNEL); // @ PWM PIN 9
	pwm_channel_disable(PWM, PIN_PWM_2NDPWM_CHANNEL);  // @ PWM PIN 8
	pwm_channel_disable(PWM, PIN_PWM_3DPWM_CHANNEL);  // @ PWM PIN 7

	/* Set PWM clock A as PWM_FREQUENCY*PERIOD_VALUE (clock B is not used) */
	pwm_clock_t clock_setting = {
		.ul_clka = PWM_FREQUENCY * PERIOD_VALUE,
		.ul_clkb = 0, // clock B is disabled
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &clock_setting);
	
	
	/* Initialize PWM channel for PWM PIN 7 */
	/* Period is left-aligned */
	g_pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	g_pwm_channel_instance.polarity = PWM_LOW;
	/* Use PWM clock A as source clock */
	g_pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_instance.ul_period = PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_instance.ul_duty = INIT_DUTY_VALUE;
	g_pwm_channel_instance.channel = PIN_PWM_3DPWM_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_instance);
		
		
	/* Initialize PWM channel for PWM PIN 8  */
	/* Period is left-aligned */
	g_pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	g_pwm_channel_instance.polarity = PWM_LOW;
	/* Use PWM clock A as source clock */
	g_pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_instance.ul_period = PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_instance.ul_duty = INIT_DUTY_VALUE;
	g_pwm_channel_instance.channel = PIN_PWM_2NDPWM_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_instance);

	/* Enable channel counter event interrupt */
	pwm_channel_enable_interrupt(PWM, PIN_PWM_2NDPWM_CHANNEL, 0);

	/* Initialize PWM channel for PWM PIN 9 */
	/* Period is left-aligned */
	g_pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	g_pwm_channel_instance.polarity = PWM_LOW;
	/* Use PWM clock A as source clock */
	g_pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_instance.ul_period = PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_instance.ul_duty = INIT_DUTY_VALUE;
	g_pwm_channel_instance.channel = PIN_PWM_1STPWM_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_instance);
	/* Disable channel counter event interrupt */
	pwm_channel_disable_interrupt(PWM, PIN_PWM_1STPWM_CHANNEL, 0);

	/* Configure interrupt and enable PWM interrupt */
	NVIC_DisableIRQ(PWM_IRQn);
	NVIC_ClearPendingIRQ(PWM_IRQn);
	NVIC_SetPriority(PWM_IRQn, 0);
	NVIC_EnableIRQ(PWM_IRQn);
	/* Enable PWM channels  */
	pwm_channel_enable(PWM, PIN_PWM_1STPWM_CHANNEL);  // @ PWM PIN 9
	pwm_channel_enable(PWM, PIN_PWM_2NDPWM_CHANNEL); // @ PWM PIN 8
	pwm_channel_enable(PWM, PIN_PWM_3DPWM_CHANNEL); //@ PWM PIN 7	
	//###########################################################################################################
	// ******MAIN LOOP*************************MAIN LOOP*******************************MAIN LOOP***************
	//###########################################################################################################
	while (1) {
	}
}

#include "servo.h"
#include <asf.h>

uint32_t currp=800;
uint32_t currt=0;

//PWM Options. Each PWM Channel can have different options
pwm_channel_t pwm_opts = {
	.ul_prescaler = PWM_CMR_CPRE_CLKA, //These options are common
	.ul_period	= PWM_PERIOD_TICKS,	//20ms period  //for all channels.
	.ul_duty = PULSE_WIDTH_CENTER_TICKS,					   //

};

void servo_config() {
	//PWM Clock options for all channels
	pwm_clock_t pwm_clock_opts = {
		.ul_clka = PWM_CLOCKSOURCE_FREQ,		//10Khz frequency = .1 ms steps
		.ul_clkb = 0,
		.ul_mck =  sysclk_get_main_hz() //main clock speed is 240Mhz!, not 120mhz
	};
	
	//configure IO pins
	pio_configure_pin( PMWL0, PMWL0_PERIPHERAL );
	pio_configure_pin( PMWL1, PMWL1_PERIPHERAL );
	
	//enable PWM clock
	pmc_enable_periph_clk(ID_PWM);
	
	//disable temporarily all channels (until everything has been properly setup)
	pwm_channel_disable(PWM, PWML0_CHANNEL);
	pwm_channel_disable(PWM, PWML1_CHANNEL);
	
	//start PWM
	pwm_init( PWM, &pwm_clock_opts );
	
	//start channels
	pwm_opts.channel = PWML0_CHANNEL;
	pwm_channel_init( PWM, &pwm_opts );
	pwm_channel_enable( PWM, PWML0_CHANNEL );
	pwm_channel_update_duty(PWM, &pwm_opts, 1000);  //neutral

	pwm_opts.channel = PWML1_CHANNEL;
	pwm_channel_init( PWM, &pwm_opts );
	pwm_channel_enable( PWM, PWML1_CHANNEL );
	pwm_channel_update_duty(PWM, &pwm_opts, 900); //neutral
}

void moveservo(unsigned short chan, int duty) {
	pwm_opts.channel = chan;
	uint32_t duty2;
	if (chan== PWML0_CHANNEL){
		currp= duty +currp;
		if (currp>2400) currp=2000;
		if (currp<400) currp=400;
		pwm_channel_update_duty(PWM, &pwm_opts, duty2=currp);
	}
	if (chan== PWML1_CHANNEL){
		currt= duty +currt;
		if (currt>2200) currt=2200;
		if (currt<300) currt=300;
		pwm_channel_update_duty(PWM, &pwm_opts, duty2=currt);
	}
	

}
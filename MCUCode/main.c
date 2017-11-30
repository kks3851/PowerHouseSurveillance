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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "wifi.h"
#include "camera.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "timer_interface.h"
#include "servo.h"
#include "adc2.h"
/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** the system state for each mode*/
typedef enum{
	STATE_SLEEP_INIT,/* wakeup from backup mode. No PLL Initialized*/
	STATE_SLEEP, /* LCD Initialization*/
	STATE_REBOOT_PIC,/* swtich MCK to PLL*/
	STATE_MOTION_DETECT,
	STATE_TRACKING,
	STATE_MANUAL,
}STATE_TYPE;

#if !defined(PMC_PCK_PRES_CLK_1)
#define PMC_PCK_PRES_CLK_1   PMC_PCK_PRES(0)
#define PMC_PCK_PRES_CLK_2   PMC_PCK_PRES(1)
#define PMC_PCK_PRES_CLK_4   PMC_PCK_PRES(2)
#define PMC_PCK_PRES_CLK_8   PMC_PCK_PRES(3)
#define PMC_PCK_PRES_CLK_16  PMC_PCK_PRES(4)
#define PMC_PCK_PRES_CLK_32  PMC_PCK_PRES(5)
#define PMC_PCK_PRES_CLK_64  PMC_PCK_PRES(6)
#endif

#ifndef PLL_DEFAULT_MUL
#define PLL_DEFAULT_MUL  7
#endif

#ifndef PLL_DEFAULT_DIV
#define PLL_DEFAULT_DIV  1
#endif

#ifndef MCK_DEFAULT_DIV
#define MCK_DEFAULT_DIV  PMC_MCKR_PRES_CLK_4
#endif

#define PIN_PUSHBUTTON_WAKEUP_PIO    PIN_PUSHBUTTON_1_PIO
#define PIN_PUSHBUTTON_WAKEUP_MASK   PIN_PUSHBUTTON_1_MASK
#define PIN_PUSHBUTTON_WAKEUP_ID     PIN_PUSHBUTTON_1_ID
#define PIN_PUSHBUTTON_WAKEUP_ATTR   PIN_PUSHBUTTON_1_ATTR

#define PIN_PUSHBUTTON_1_PIN	PIO_PA18
#define PIN_PUSHBUTTON_1_MASK	PIO_PA18_IDX
#define PIN_PUSHBUTTON_1_PIO	PIOA
#define PIN_PUSHBUTTON_1_ID		ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE	PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR	PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

/** Current MCK in Hz */
uint32_t g_ul_current_mck;

/** Button pressed flag */
volatile uint32_t g_ul_button_pressed = 0;

volatile STATE_TYPE cur_state = STATE_SLEEP_INIT;

uint8_t start_capture(void)
{
	NVIC_EnableIRQ((IRQn_Type)OV2640_VSYNC_ID);
	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait vsync_flag = 1 before start process
	 */
	while (!vsync_flag){
		/* Enable vsync interrupt*/
		pio_enable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);
	
		//EnterSleepMode();
	
		/* Disable vsync interrupt*/
		pio_disable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);
	}
	
	/* Enable pio capture*/
	pio_capture_enable(OV2640_DATA_BUS_PIO);
	

	/* Capture data and send it to buffer using PDC */
	pio_capture_to_buffer(OV2640_DATA_BUS_PIO, image_buf, IM_BUF_LEN/4);

	//OV2640_DATA_BUS_PIO-> PIO_PCIER |= PIO_PCIER_RXBUFF;

	/* Wait end of capture*/
	while (!((OV2640_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_ENDRX) ==	PIO_PCIMR_ENDRX));

	/* Disable pio capture*/
	pio_capture_disable(OV2640_DATA_BUS_PIO);

	/* Reset vsync flag*/
	vsync_flag = false;
	
	if (find_image_len()) return 1; // Success
	else return 0; // Failure
}
static void button_handler(uint32_t ul_id, uint32_t ul_mask)
{
	if (PIN_PUSHBUTTON_WAKEUP_ID == ul_id &&
	PIN_PUSHBUTTON_WAKEUP_MASK == ul_mask) {
		
	}
	cur_state= STATE_REBOOT_PIC;
}

static void configure_button(void)
{
	
	/* Configure PIO clock. */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_WAKEUP_ID);
	/* Initialize PIO interrupt handlers, see PIO definition in board.h. */
	pio_handler_set(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_WAKEUP_ID,
	PIN_PUSHBUTTON_1_PIN, PIN_PUSHBUTTON_WAKEUP_ATTR,
	button_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)PIN_PUSHBUTTON_WAKEUP_ID);

	/* Enable PIO line interrupts. */
	pio_enable_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO,
	PIN_PUSHBUTTON_1_PIN);
}

static void reconfigure_console(uint32_t ul_mck, uint32_t ul_baudrate)
{
	const sam_uart_opt_t usart_console_settings =
	{ ul_mck, ul_baudrate, US_MR_PAR_NO};

	/* Configure PMC */
	pmc_enable_periph_clk(WIFI_USART_ID);

	/* Configure PIO */
	pio_configure_pin_group(PINS_WIFI_USART_PIO, PINS_WIFI_USART,
	PINS_WIFI_USART_FLAGS);

	/* Configure UART */
	configure_usart_wifi();
}

	
static void test_sleep_mode(void)
{
	/* Configure button for waking up sleep mode */
	configure_button();

	/* Disable UART */
	pmc_disable_periph_clk(WIFI_USART_ID);

	/* Enter into sleep Mode */
	pmc_enable_sleepmode(0);

}

int main (void)
{
	// System initialization
	sysclk_init();
	wdt_disable(WDT);
	board_init();
	
	// Timeout timer initialization
	configure_tc();
	tc_start(TC0, 0);
	
	// WiFi initialization
	configure_usart_wifi();
	configure_wifi_comm_pin();
	configure_wifi_web_setup_pin();
	
	write_wifi_command("set sy i g wlan 20\r\n", 3);
	write_wifi_command("set sy i g network 21\r\n", 3);
	write_wifi_command("set sy i g softap 22\r\n", 3);
	write_wifi_command("set sy c g 4\r\n", 3);
	write_wifi_command("set wl n o 3\r\n", 3);
	write_wifi_command("set bus.command.rx_bufsize 5000\r\n", 3);
	write_wifi_command("set system.cmd.prompt_enabled 0\r\n",3);
	write_wifi_command("set system.cmd.echo off\r\n",3);
	write_wifi_command("set wlan.auto_join.enabled 1\r\n", 3);
	write_wifi_command("save\r\n", 3);
	
	
	// Reset WiFi module and wait for connection
	ioport_set_pin_level(WIFI_RESET_PIN, false);
	delay_ms(100);
	ioport_set_pin_level(WIFI_RESET_PIN, true);
	wifi_web_setup_flag = false;
	while (!ioport_get_pin_level(WIFI_NETWORK_PIN)) {
		if (wifi_web_setup_flag) {
			usart_write_line(WIFI_USART, "setup web\r\n");
			wifi_web_setup_flag = false;
		}
	}
	
	// Camera initialization
	init_camera();
	ioport_set_pin_level(CAM_RST_PIN, true);
	delay_ms(3000);
	configure_camera();	delay_ms(100);
	

	
	servo_config();
	write_wifi_command("tcpc 10.106.6.149 1025\r\n",10);

	while (1) { // Main loop
		switch (cur_state){
			case STATE_SLEEP_INIT:
				test_sleep_mode();
				cur_state=STATE_SLEEP;
			
			case STATE_SLEEP:
				break;
			
			case STATE_REBOOT_PIC: 
				reconfigure_console(g_ul_current_mck, WIFI_USART_BAUDRATE);
				servo_config();
				delay_ms(10);
				
			case STATE_MOTION_DETECT: 
				write_wifi_command("tcpc 10.106.6.149 1025\r\n",10);
				while (ioport_get_pin_level(PIR_PIN)) {
					if(start_capture()) write_image_to_file();
					write_wifi_command("read 0 20\r\n",10);
				}
				usart_write_line(WIFI_USART, "close all\r\n");
				pwm_channel_disable(PWM, PWML0_CHANNEL);
				pwm_channel_disable(PWM, PWML1_CHANNEL);
				cur_state= STATE_SLEEP_INIT;
				break;
			
			case STATE_TRACKING: 
				pio_disable_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_1_PIN);
				while (track_mode) {
					if(start_capture()) write_image_to_file();
				}
				pio_enable_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_1_PIN);
				cur_state= STATE_MOTION_DETECT;
			break;
			
			case STATE_MANUAL: 
				pio_disable_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_1_PIN);
				while (man_mode){
					if(start_capture()) write_image_to_file();
				}
				pio_enable_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_1_PIN);
				cur_state= STATE_MOTION_DETECT;
			break;
		}
	}
}


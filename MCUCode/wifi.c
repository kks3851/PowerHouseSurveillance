/*
 * wifi.c
 *
 * Created: 11/27/2016 10:48:50 AM
 *  Author: Ilya
 */ 

#include "camera.h"
#include "wifi.h"
#include "timer_interface.h"
#include "servo.h"
#include <string.h>
#include <stdio.h>

volatile uint32_t received_byte_wifi = 0;
volatile bool new_rx_wifi = false;

volatile bool wifi_web_setup_flag = false;
volatile unsigned int input_pos_wifi = 0;
volatile bool wifi_comm_success = false;
volatile bool wifi_file_create_success = false;
volatile bool wifi_file_delete_success = false;
volatile bool wifi_file_open_success = false;
volatile bool wifi_file_write_success = false;
volatile bool wifi_streams_unavailable = false;
volatile bool wifi_stream0_available = false;
volatile bool wifi_stream1_available = false;
volatile bool wifi_stream2_available = false;
volatile bool wifi_stream3_available = false;
volatile bool wifi_stream4_available = false;
volatile bool wifi_stream5_available = false;
volatile bool wifi_stream6_available = false;
volatile bool wifi_stream7_available = false;
volatile bool man_mode = false;
volatile bool track_mode= false;

void WIFI_USART_HANDLER(void)
{
	uint32_t ul_status;

	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(WIFI_USART, &received_byte_wifi);
		new_rx_wifi = true;
		processIncomingByte_wifi((uint8_t)received_byte_wifi);
	}
}

void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	wifi_comm_success = true;
	process_data_wifi();
	for (int jj=0;jj<MAX_INPUT_WIFI;jj++) input_line_wifi[jj] = 0;
	input_pos_wifi = 0;
}

void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	wifi_web_setup_flag = true;
}

void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t wifi_usart_settings = {
		WIFI_USART_BAUDRATE,
		WIFI_USART_CHAR_LENGTH,
		WIFI_USART_PARITY,
		WIFI_USART_STOP_BITS,
		WIFI_USART_MODE,
		/* This field is only used in IrDA mode. */
		0
	};
	
	/* Get system clock. */
	ul_sysclk = sysclk_get_cpu_hz();
	
	pmc_enable_periph_clk(WIFI_USART_ID);
	
	//usart_init_hw_handshaking(WIFI_USART,&wifi_usart_settings,ul_sysclk);
	usart_init_rs232(WIFI_USART, &wifi_usart_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);

	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}

void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_NUM, WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_NUM);
}

void configure_wifi_web_setup_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_WEB_SETUP_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_WEB_SETUP_PIO, WIFI_WEB_SETUP_ID, WIFI_WEB_SETUP_PIN_NUM, WIFI_WEB_SETUP_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_WEB_SETUP_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_WEB_SETUP_PIO, WIFI_WEB_SETUP_PIN_NUM);
}

void write_wifi_command(char* comm, uint8_t cnt)
{
	wifi_comm_success = false;
	usart_write_line(WIFI_USART, comm);
	counts = 0;
	while(!wifi_comm_success && counts<cnt);
}


void write_image_to_file(void)
{
	if (len_image == 0) return;
	else {
		char file_create_string[100];
		//
		//wifi_file_delete_success = false;
		//usart_write_line(WIFI_USART, "fde image.jpg\r\n");
		//counts = 0;
		//while(!wifi_file_delete_success && counts<10);
		//
		//wifi_file_create_success = false;
		//sprintf(file_create_string,"fcr image.jpg %d\r",len_image);
		//usart_write_line(WIFI_USART, file_create_string);
		//
		//for (uint32_t jj=0;jj<len_image;jj++) {
			//usart_putchar(WIFI_USART, image_buf[soi_addr+jj]);
		//}
		//usart_write_line(WIFI_USART, "write 0 1\rh");		
		
		sprintf(file_create_string, "hpo -o http://10.106.6.149:1880/webcam application/octet-stream\r\n");
		usart_write_line(WIFI_USART, file_create_string);
		sprintf(file_create_string, "write 1 %d\r", len_image);
		usart_write_line(WIFI_USART, file_create_string); //"write 0 2\rhi");//file_create_string);
		for (uint32_t jj=0;jj<len_image;jj++) {
			usart_putchar(WIFI_USART, image_buf[soi_addr+jj]);
		}
		usart_write_line(WIFI_USART, "hre 1\r\n");
		//usart_write_line(WIFI_USART, "list\r\n");
		usart_write_line(WIFI_USART, "close 1\r\n");
		usart_write_line(WIFI_USART, "read 0 10\r\n");
		
	}
}

volatile void processIncomingByte_wifi(uint8_t inByte)
{
	
	static char input_line [MAX_INPUT_WIFI];
	static unsigned int input_pos = 0;
	
	input_line_wifi[input_pos_wifi++] = inByte;
}

volatile void process_data_wifi ()
{
	
	if (strstr(input_line_wifi, "File deleted") || strstr(input_line_wifi, "File not found")) {
		wifi_file_delete_success = true;
	}
	else if (strstr(input_line_wifi, "panleft")) {
		moveservo(PWML0_CHANNEL, 100);
	}
	else if (strstr(input_line_wifi, "panright")){
		moveservo(PWML0_CHANNEL, -100);
	}
	else if (strstr(input_line_wifi,"File created") || strstr(input_line_wifi,"File already exists")) {
		wifi_file_create_success = true;
	}
	else if (strstr(input_line_wifi, "Opened")) {
		wifi_file_open_success = true;
	}
	else if (strstr(input_line_wifi, "Success")) {
		wifi_file_write_success = true;
	}
	else if (strstr(input_line_wifi, "None")) {
		wifi_streams_unavailable = true;
	}
	else if (strstr(input_line_wifi, "manual")) {
		if (man_mode) man_mode = false;
		else man_mode = true;
	}
	else if (strstr(input_line_wifi, "track")) {
		if (track_mode) track_mode = false;
		else track_mode = true;
	}
	else if (strstr(input_line_wifi, "panleft")) {
		moveservo(PWML0_CHANNEL, 100);
	}
	else if (strstr(input_line_wifi, "panright")){
		moveservo(PWML0_CHANNEL, -100);
	}
	else if (strstr(input_line_wifi, "tiltup")) {
		moveservo(PWML1_CHANNEL, 100);
	}
	else if (strstr(input_line_wifi, "tiltdown")){
		moveservo(PWML1_CHANNEL, -100);
	}

	if (strstr(input_line_wifi, "# 0")) {
		wifi_stream0_available = true;
	}
	if (strstr(input_line_wifi, "# 1")) {
		wifi_stream1_available = true;
	}
	if (strstr(input_line_wifi, "# 2")) {
		wifi_stream2_available = true;
	}
	if (strstr(input_line_wifi, "# 3")) {
		wifi_stream3_available = true;
	}
	if (strstr(input_line_wifi, "# 4")) {
		wifi_stream4_available = true;
	}
	if (strstr(input_line_wifi, "# 5")) {
		wifi_stream5_available = true;
	}
	if (strstr(input_line_wifi, "# 6")) {
		wifi_stream6_available = true;
	}
	if (strstr(input_line_wifi, "# 7")) {
		wifi_stream7_available = true;
	}
	if (strstr(input_line_wifi,"Unknown command")) {
		return;
	}
}

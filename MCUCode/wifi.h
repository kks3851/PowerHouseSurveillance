/*
 * wifi.h
 *
 * Created: 11/27/2016 10:48:40 AM
 *  Author: Ilya
 */ 


#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>
#include <stdio.h>

#define WIFI_USART					USART0
#define WIFI_USART_ID				ID_USART0
#define WIFI_USART_BAUDRATE			115200
#define WIFI_USART_HANDLER			USART0_Handler
#define WIFI_USART_IRQn				USART0_IRQn
#define WIFI_USART_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define WIFI_USART_PARITY			US_MR_PAR_NO
#define WIFI_USART_STOP_BITS		US_MR_NBSTOP_1_BIT
#define WIFI_USART_MODE				US_MR_CHMODE_NORMAL

#define PINS_WIFI_USART				(PIO_PA5A_RXD0 | PIO_PA6A_TXD0)
#define PINS_WIFI_USART_FLAGS		(PIO_PERIPH_A | PIO_DEFAULT)
#define PINS_WIFI_USART_MASK		(PIO_PA5A_RXD0 | PIO_PA6A_TXD0)
#define PINS_WIFI_USART_PIO			PIOA
#define PINS_WIFI_USART_ID			ID_PIOA
#define PINS_WIFI_USART_TYPE		PIO_PERIPH_A
#define PINS_WIFI_USART_ATTR		PIO_DEFAULT
#define PINS_WIFI_USART_FLOW		(PIO_PA7A_RTS0 | PIO_PA8A_CTS0)

/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

/** All interrupt mask. */
#define ALL_INTERRUPT_MASK  0xffffffff

#define WIFI_COMM_PIN_NUM			PIO_PB10
#define WIFI_COMM_PIO				PIOB
#define WIFI_COMM_ID				ID_PIOB
#define WIFI_COMM_MASK				PIO_PB10_IDX
#define WIFI_COMM_ATTR				PIO_IT_RISE_EDGE

#define WIFI_WEB_SETUP_PIN_NUM			PIO_PA22
#define WIFI_WEB_SETUP_PIO				PIOA
#define WIFI_WEB_SETUP_ID				ID_PIOA
#define WIFI_WEB_SETUP_MASK				PIO_PA22_IDX
#define WIFI_WEB_SETUP_ATTR				PIO_IT_FALL_EDGE


#define MAX_PACKET_LEN 2048
#define MAX_INPUT_WIFI 1000
volatile uint32_t received_byte_wifi;

volatile bool wifi_web_setup_flag;
volatile char input_line_wifi[MAX_INPUT_WIFI];
volatile unsigned int input_pos_wifi;
volatile bool wifi_comm_success;
volatile bool wifi_file_delete_success;
volatile bool wifi_file_create_success;
volatile bool wifi_file_open_success;
volatile bool wifi_file_write_success;
volatile bool wifi_streams_unavailable;
volatile bool wifi_stream0_available;
volatile bool wifi_stream1_available;
volatile bool wifi_stream2_available;
volatile bool wifi_stream3_available;
volatile bool wifi_stream4_available;
volatile bool wifi_stream5_available;
volatile bool wifi_stream6_available;
volatile bool wifi_stream7_available;

void configure_usart_wifi(void);
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void configure_wifi_comm_pin(void);
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask);
void configure_wifi_web_setup_pin(void);

void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_file(void);

volatile void processIncomingByte_wifi(uint8_t inByte);
volatile void process_data_wifi(void);


#endif /* WIFI_H_ */
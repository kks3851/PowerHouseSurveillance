/*
 * i2c_interface.h
 *
 * Created: 11/27/2016 10:35:40 AM
 *  Author: Ilya
 */ 


#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_

#include <asf.h>
#include <string.h>
#include <stdio.h>

/* TWI clock frequency in Hz */
#define TWI_CLK     (100000UL)
/* TWI board defines. */
#define SC_TWI_ID				ID_TWI0
#define SC_TWI                  TWI0
#define SC_TWI_IRQn             TWI0_IRQn

/** TWI0 pin definitions */
#define TWI0_DATA_GPIO   PIO_PA3_IDX
#define TWI0_DATA_FLAGS  (PIO_PERIPH_A | PIO_PULLUP)
#define TWI0_CLK_GPIO    PIO_PA4_IDX
#define TWI0_CLK_FLAGS   (PIO_PERIPH_A | PIO_PULLUP)

#define ADDR_OV2640				0x30

/** PCK1 */
#define PIN_PCK1                       (PIO_PA17_IDX)
#define PIN_PCK1_FLAGS                 (PIO_PERIPH_B | PIO_DEFAULT)

/******************************* Image sensor definition
 *********************************/
/** HSYNC pin */
#define PIN_OV_HSYNC                   { PIO_PA16, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_IT_RISE_EDGE }

/** VSYNC pin */
#define PIN_OV_VSYNC                   { PIO_PA15, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_IT_RISE_EDGE }

/** OV_HSYNC pin definition */
#define OV_HSYNC_GPIO                  PIO_PA16_IDX
#define OV_HSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_HSYNC_MASK                  PIO_PA16
#define OV_HSYNC_PIO                   PIOA
#define OV_HSYNC_ID                    ID_PIOA
#define OV_HSYNC_TYPE                  PIO_PULLUP

/** OV_VSYNC pin definition */
#define OV_VSYNC_GPIO                  PIO_PA15_IDX
#define OV_VSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_VSYNC_MASK                  PIO_PA15
#define OV_VSYNC_PIO                   PIOA
#define OV_VSYNC_ID                    ID_PIOA
#define OV_VSYNC_TYPE                  PIO_PULLUP

/** OV Data Bus pins */
#define OV_DATA_BUS_D2                 PIO_PA24_IDX
#define OV_DATA_BUS_D3                 PIO_PA25_IDX
#define OV_DATA_BUS_D4                 PIO_PA26_IDX
#define OV_DATA_BUS_D5                 PIO_PA27_IDX
#define OV_DATA_BUS_D6                 PIO_PA28_IDX
#define OV_DATA_BUS_D7                 PIO_PA29_IDX
#define OV_DATA_BUS_D8                 PIO_PA30_IDX
#define OV_DATA_BUS_D9                 PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT

// Image sensor VSYNC pin.
#define OV2640_VSYNC_PIO		       OV_VSYNC_PIO
#define OV2640_VSYNC_ID		           OV_VSYNC_ID
#define OV2640_VSYNC_MASK              OV_VSYNC_MASK
#define OV2640_VSYNC_TYPE              OV_VSYNC_TYPE
// Image sensor data pin.
#define OV2640_DATA_BUS_PIO            OV_DATA_BUS_PIO
#define OV2640_DATA_BUS_ID             OV_DATA_BUS_ID


/******************************* Parallel Capture definition
 *********************************/
/** Parallel Capture Mode Data Enable1 */
#define PIN_PIODCEN1                   PIO_PA15

/** Parallel Capture Mode Data Enable2 */
#define PIN_PIODCEN2                   PIO_PA16

// Image buffer array size
#define IM_BUF_LEN 100000

// Variables
volatile uint8_t image_buf[IM_BUF_LEN];
volatile bool vsync_flag;
uint32_t len_image;
uint32_t soi_addr;

// Functions
void init_vsync_interrupts(void);
void configure_twi(void);
void pio_capture_init(Pio *p_pio, uint32_t ul_id);
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size);
void init_camera(void);
void configure_camera(void);
uint8_t start_capture(void);
uint8_t find_image_len(void);
void vsync_handler(uint32_t ul_id, uint32_t ul_mask);


#endif /* I2C_INTERFACE_H_ */
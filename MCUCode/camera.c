/*
 * i2c_interface.c
 *
 * Created: 11/27/2016 10:35:27 AM
 *  Author: Ilya
 */ 

#include "camera.h"
#include "ov2640.h"

volatile bool vsync_flag = false;
uint32_t len_image = 0;

void vsync_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	vsync_flag = true;
}

void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(OV2640_VSYNC_PIO, OV2640_VSYNC_ID, OV2640_VSYNC_MASK,
			OV2640_VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV2640_VSYNC_ID);
}

void configure_twi(void)
{
	twi_options_t opt;
	
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	/* Enable TWI peripheral */
	pmc_enable_periph_clk(SC_TWI_ID);
	
	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(SC_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(SC_TWI_IRQn);
	NVIC_ClearPendingIRQ(SC_TWI_IRQn);
	NVIC_SetPriority(SC_TWI_IRQn, 0);
	NVIC_EnableIRQ(SC_TWI_IRQn);
}

void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable peripheral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable pio capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable drdy interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 8bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);
}

uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
		}
	else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
		}
	else {
		return 0;
	}
}

void init_camera(void)
{
	// Configure camera clock
	pmc_enable_pllbck(7, 0x1, 1);
	gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1));
	
	// Configure camera pins
	gpio_configure_pin(OV_HSYNC_GPIO, OV_HSYNC_FLAGS);
	gpio_configure_pin(OV_VSYNC_GPIO, OV_VSYNC_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D8, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D9, OV_DATA_BUS_FLAGS);
	
	/* Init Vsync handler*/
	init_vsync_interrupts();

	/* Init PIO capture*/
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);
	
	configure_twi();
}

void configure_camera(void)
{
	/* ov2640 Initialization */
	// First, make sure reg 0xFF=1
	const uint8_t cont_reg_val = 1;
	twi_packet_t init_packet = {
		.addr         = 0xFF,           // TWI slave memory address data
		.addr_length  = 1,				// TWI slave memory address data size
		.chip         = ADDR_OV2640,    // TWI slave bus address
		.buffer       = &cont_reg_val,  // transfer data destination buffer
		.length       = 1               // transfer data size (bytes)
	};
	ov_write_reg(SC_TWI, &init_packet);
	while (ov_init(SC_TWI) == 1) {
	}

	/* ov2640 configuration */
	ov_configure(SC_TWI, JPEG_INIT);
	ov_configure(SC_TWI, YUV422);
	ov_configure(SC_TWI, JPEG);
	ov_configure(SC_TWI, JPEG_640x480);
}



uint8_t find_image_len(void)
{
	bool eoi = false;
	bool soi = false;
	uint32_t ii = 0;
	bool jpeg_ctrl_flag = false;
	
	while (!eoi && ii<IM_BUF_LEN) {
		if (jpeg_ctrl_flag) {
			jpeg_ctrl_flag = false;
			if (image_buf[ii]==217 && soi==true) {
				len_image = ii + 1 - soi_addr;
				eoi = true;
			}
			else if (image_buf[ii]==216) {
				soi = true;
				soi_addr = ii-1;
			}
		}
		if (image_buf[ii]==255) {
			jpeg_ctrl_flag = true;
		}
		ii++;
	}
	
	if (eoi) return 1; // Success
	else return 0; // Failure
}

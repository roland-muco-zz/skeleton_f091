/*
 * \file	bsp.c
 * \brief	supporting code, always needed
 * \date	Nov 19, 2020
 * \author	roland van straten
 *
 */

#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "bsp.h"
#include "crc.h"
#include "i2c.h"
#include "usart.h"
#include "main.h"


void bsp_hwcrc_reinit(crc_modes_t selector);

/* implementation */

/* by the way: you can use these also to serve as callback routines using the Bosch sensor device drivers */

/**
 * \brief	wait for what
 */
void bsp_delay_ms(uint32_t period)
{
    HAL_Delay(period);
}

/**
 * \brief	data is pouring in from the I2C1
 */
int8_t bsp_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; // return 0 for Success, non-zero for failure
    /* set the I2C addr + R/W bit */
    dev_id <<= 1;
    /* assume 8 bit writes, check I2C_MEMADD_SIZE_16BIT for memories like M24M02 etc. */
    if (HAL_I2C_Mem_Read(&hi2c1, dev_id, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 1000) != HAL_OK)
    {
      rslt = -1;
    }
    return rslt;
}

/**
 * \brief	write a handful of characters to I2C1
 */
int8_t bsp_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; // return 0 for Success, non-zero for failure
    dev_id <<= 1;
    if (HAL_I2C_Mem_Write(&hi2c1, dev_id, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 1000) != HAL_OK)
    {
      rslt = -1;
    }
    return rslt;
}

/**
 * \brief	I have a bright idea
 */
void bsp_led_on(void)
{
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/**
 * \brief	turm off the light and go home
 */
void bsp_led_off(void)
{
	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

/**
 * \brief	led is on or is it off, no it is on
 */
void bsp_led_toggle(void)
{
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

uint8_t bsp_button(void)
{
	return (HAL_GPIO_ReadPin(USR_BTN_GPIO_Port, USR_BTN_Pin) != GPIO_PIN_RESET) ? 1 : 0 ;
}

/**
 * \brief	init the console
 */
void bsp_console_init(void)
{
	/* turn off buffering, so output will be immediate */
	setvbuf(stdout, NULL, _IONBF, 0);
}

/**
 * \brief	output char via usart2 to enable printf
 */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

/**
 * \brief	input char from USART2 to be able to use scanf
 */
int __io_getchar(void)
{
	static uint8_t buf[1];
	HAL_UART_Receive(&huart2, buf, 1, HAL_MAX_DELAY);
	return buf[0];
}

/**
 * \brief	welcome message
 * 			should be a random message (like motd on unix)
 */
void bsp_welcome(void)
{
	/* famous quote from Emerson, Lake & Palmer concert */
	printf("Welcome friends to the show that never ends...\r\n");
}


#if(1)
/*******************
 * CRC HARDWARE
 *
 * Even though all STM32's have a hardware CRC provision, it seems to be not very popular. Why spend
 * so much space in flash or computation time to crc ehile there is a number cruncher in the micro-controller
 *
 * https://reveng.sourceforge.io/crc-catalogue/17plus.htm#crc.cat.crc-32c
 * https://www.lammertbies.nl/comm/info/crc-calculation
 *
 *******************/

/**
 * \brief	test
 *
 */
void bsp_hwcrc_test(void)
{
	uint8_t data[9] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};

	printf("STM32 Hardware CRC Test\r\n");

	uint16_t chcksm = 0;
	for(int i = 0; i<9; i++)
	{
		chcksm = (chcksm + data[i]) % 256;
	}
	printf("Checksum MODULO 256 %x\r\n", chcksm);

	bsp_hwcrc_reinit(CRC16_XMODEM);
	printf("CRC16 XMODEM %lx\r\n", HAL_CRC_Calculate(&hcrc, (uint32_t*)&data[0], 9) );

	bsp_hwcrc_reinit(CRC16_MODBUS);
	printf("CRC16 MODBUS %lx\r\n", HAL_CRC_Calculate(&hcrc, (uint32_t*)&data[0], 9) );
}


/**
 * \brief	reconfigure the CRC peripheral
 *
 */
void bsp_hwcrc_reinit(crc_modes_t selector)
{
	__HAL_RCC_CRC_CLK_ENABLE();

	hcrc.Instance = CRC;

	switch(selector)
	{
	case 0:
	    /* width=16
	     * poly=0x1021
	     * init=0x0000
	     * refin=false
	     * refout=false
	     * xorout=0x0000
	     * check=0x31c3
	     * residue=0x0000
	     * name="CRC-16/XMODEM"
	     */

/* width */
//			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_UNDEFINED;
			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
//			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_HALFWORDS;
//			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_WORDS;

/* poly - default polynomial 0x04C11DB7 */
//			hcrc.Init.CRCLength = CRC_POLYLENGTH_32B;
			hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
//			hcrc.Init.CRCLength = CRC_POLYLENGTH_8B;
//			hcrc.Init.CRCLength = CRC_POLYLENGTH_7B;

			hcrc.Init.GeneratingPolynomial = 0x1021;
	    	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
//			hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;

/* init */	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
//			hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
	    	hcrc.Init.InitValue = 0;

/* refin */	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
//			hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
//			hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_HALFWORD;
//			hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_WORD;

/* refout */hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
//			hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;

	break;

	case 1:
		/* width=16
		 * poly=0x8005
		 * init=0xffff
		 * refin=true
		 * refout=true
		 * xorout=0x0000
		 * check=0x4b37
		 * residue=0x0000
		 * name="CRC-16/MODBUS"
		 */

/* width */
//			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_UNDEFINED;
			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
//			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_HALFWORDS;
//			hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_WORDS;

/* poly - default polynomial 0x04C11DB7 */
//			hcrc.Init.CRCLength = CRC_POLYLENGTH_32B;
			hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
//			hcrc.Init.CRCLength = CRC_POLYLENGTH_8B;
//			hcrc.Init.CRCLength = CRC_POLYLENGTH_7B;

			hcrc.Init.GeneratingPolynomial = 0x8005;
			hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
//			hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;

/* init */	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
//			hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
			hcrc.Init.InitValue = 0xFFFF;

/* refin */
//			hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
			hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
//			hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_HALFWORD;
//			hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_WORD;

/* refout */
//			hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
			hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
	break;
	}

    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        Error_Handler();
    }
}
#endif

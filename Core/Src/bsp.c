/*
 * \file	bsp.c
 * \brief	supporting code, always needed
 * \date	Nov 19, 2020
 * \author	roland van straten
 *
 *
 *	\note	UART5 is used instead of UART2 for console output, see the #if(0)
 *
 */

#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "bsp.h"
#include "i2c.h"
#include "usart.h"
#include "main.h"



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


/**
 * \brief	init the console
 */
void bsp_console_init(void)
{
	/* turn off buffering, so output will be immediate */
	setvbuf(stdout, NULL, _IONBF, 0);
}

#if(0)
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
#else
/**
 * \brief	output char via usart5 to enable printf
 */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart5, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

/**
 * \brief	input char from USART5 to be able to use scanf
 */
int __io_getchar(void)
{
	static uint8_t buf[1];
	HAL_UART_Receive(&huart5, buf, 1, HAL_MAX_DELAY);
	return buf[0];
}
#endif


/**
 * \brief	welcome message
 * 			should be a random message (like motd on unix)
 */
void bsp_welcome(void)
{
	/* famous quote from Emerson, Lake & Palmer concert */
	printf("Welcome friends to the show that never ends...\r\n");
}


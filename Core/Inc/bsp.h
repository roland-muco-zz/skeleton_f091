/*
 * \file	bsp.h
 * \brief	supporting code, always needed
 * \date	Nov 19, 2020
 * \author	roland van straten
 *
 */

#ifndef INC_BSP_H_
#define INC_BSP_H_


/* hardware CRC */
typedef enum e_crc_modes {

	CRC16_XMODEM = 0,
	CRC16_MODBUS = 1,

} crc_modes_t;


/* hardware CRC */
void bsp_hwcrc_reinit(uint8_t selector);
void bsp_hwcrc_test(void);

/* time delay */
void bsp_delay_ms(uint32_t period);

/* i2c1 */
int8_t bsp_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t bsp_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

/* led */
void bsp_led_on(void);
void bsp_led_off(void);
void bsp_led_toggle(void);

/* push button */
uint8_t bsp_button(void);

/* console usart */
void bsp_console_init(void);
int __io_putchar(int ch);
int __io_getchar(void);
void bsp_welcome(void);


#endif /* INC_BSP_H_ */

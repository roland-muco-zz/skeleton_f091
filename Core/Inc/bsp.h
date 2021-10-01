/*
 * \file	bsp.h
 * \brief	supporting code, always needed
 * \date	Nov 19, 2020
 * \author	roland van straten
 *
 */

#ifndef INC_BSP_H_
#define INC_BSP_H_


/* time delay */
void bsp_delay_ms(uint32_t period);

/* i2c1 */
int8_t bsp_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t bsp_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

/* led */
void bsp_led_on(void);
void bsp_led_off(void);
void bsp_led_toggle(void);


/* console usart */
void bsp_console_init(void);
int __io_putchar(int ch);
int __io_getchar(void);
void bsp_welcome(void);


#endif /* INC_BSP_H_ */

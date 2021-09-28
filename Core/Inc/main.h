/**
 * \file	main.h
 * \brief	common defines of the application
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f0xx_hal.h"


void Error_Handler(void);


#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


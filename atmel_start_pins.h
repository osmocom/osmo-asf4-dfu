/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAME54 has 14 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8
#define GPIO_PIN_FUNCTION_J 9
#define GPIO_PIN_FUNCTION_K 10
#define GPIO_PIN_FUNCTION_L 11
#define GPIO_PIN_FUNCTION_M 12
#define GPIO_PIN_FUNCTION_N 13

/** LED pin to indicate system state */
#if defined(SAME54_XPLAINED_PRO)
#define LED_SYSTEM GPIO(GPIO_PORTC, 18)
#elif defined(SYSMOOCTSIM)
#define LED_SYSTEM GPIO(GPIO_PORTC, 26)
#endif

/** User button to force DFU bootloader (connected to ground when pressed) */
#if defined(SAME54_XPLAINED_PRO)
#define BUTTON_FORCE_DFU GPIO(GPIO_PORTB, 31)
#elif defined(SYSMOOCTSIM)
#define BUTTON_FORCE_DFU GPIO(GPIO_PORTC, 14)
#endif

/** USB D+/D- pins */
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)

#endif // ATMEL_START_PINS_H_INCLUDED

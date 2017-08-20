/*
 * gpio.h
 *
 *  Created on: 13 août 2017
 *      Author: jfellus
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <msp430.h>
#include "time.h"
#include <sys/types.h>
#include "uartio.h"


typedef uint8_t bool;

// Internals

volatile uint8_t* _PxIFG[] = { 0,&P1IFG, &P2IFG };
volatile uint8_t* _PxOUT[] = { 0,&P1OUT, &P2OUT, &P3OUT, &P4OUT, &P5OUT, &P6OUT, &P7OUT, &P8OUT };
volatile const uint8_t* _PxIN[] = { 0,&P1IN, &P2IN, &P3IN, &P4IN, &P5IN, &P6IN, &P7IN, &P8IN };
volatile uint8_t* _PxIE[] = { 0,&P1IE, &P2IE };
volatile uint8_t* _PxIES[] = { 0,&P1IES, &P2IES };
volatile uint8_t* _PxREN[] = { 0,&P1REN, &P2REN, &P3REN, &P4REN, &P5REN, &P6REN, &P7REN, &P8REN };
volatile uint8_t* _PxSEL[] = { 0,&P1SEL, &P2SEL, &P3SEL, &P4SEL, &P5SEL, &P6SEL, &P7SEL, &P8SEL };
volatile uint8_t* _PxDIR[] = { 0,&P1DIR, &P2DIR, &P3DIR, &P4DIR, &P5DIR, &P6DIR, &P7DIR, &P8DIR };

#define PxIFG(p) (*( _PxIFG[(p >> 8)] ))
#define PxOUT(p) (*( _PxOUT[(p >> 8)] ))
#define PxIN(p) (*( _PxIN[(p >> 8)] ))
#define PxIE(p) (*( _PxIE[(p >> 8)] ))
#define PxIES(p) (*( _PxIES[(p >> 8)] ))
#define PxREN(p) (*( _PxREN[(p >> 8)] ))
#define PxSEL(p) (*( _PxSEL[(p >> 8)] ))
#define PxDIR(p) (*( _PxDIR[(p >> 8)] ))
#define _PIN(p) ((uint8_t)(p))

#define P1_0 (0x100 | BIT0)
#define P1_1 (0x100 | BIT1)
#define P1_2 (0x100 | BIT2)
#define P1_3 (0x100 | BIT3)
#define P1_4 (0x100 | BIT4)
#define P1_5 (0x100 | BIT5)
#define P1_6 (0x100 | BIT6)
#define P1_7 (0x100 | BIT7)

#define P2_0 (0x200 | BIT0)
#define P2_1 (0x200 | BIT1)
#define P2_2 (0x200 | BIT2)
#define P2_3 (0x200 | BIT3)
#define P2_4 (0x200 | BIT4)
#define P2_5 (0x200 | BIT5)
#define P2_6 (0x200 | BIT6)
#define P2_7 (0x200 | BIT7)

#define P3_0 (0x300 | BIT0)
#define P3_1 (0x300 | BIT1)
#define P3_2 (0x300 | BIT2)
#define P3_3 (0x300 | BIT3)
#define P3_4 (0x300 | BIT4)
#define P3_5 (0x300 | BIT5)
#define P3_6 (0x300 | BIT6)
#define P3_7 (0x300 | BIT7)

#define P4_0 (0x400 | BIT0)
#define P4_1 (0x400 | BIT1)
#define P4_2 (0x400 | BIT2)
#define P4_3 (0x400 | BIT3)
#define P4_4 (0x400 | BIT4)
#define P4_5 (0x400 | BIT5)
#define P4_6 (0x400 | BIT6)
#define P4_7 (0x400 | BIT7)

#define P5_0 (0x500 | BIT0)
#define P5_1 (0x500 | BIT1)
#define P5_2 (0x500 | BIT2)
#define P5_3 (0x500 | BIT3)
#define P5_4 (0x500 | BIT4)
#define P5_5 (0x500 | BIT5)
#define P5_6 (0x500 | BIT6)
#define P5_7 (0x500 | BIT7)

#define P6_0 (0x600 | BIT0)
#define P6_1 (0x600 | BIT1)
#define P6_2 (0x600 | BIT2)
#define P6_3 (0x600 | BIT3)
#define P6_4 (0x600 | BIT4)
#define P6_5 (0x600 | BIT5)
#define P6_6 (0x600 | BIT6)
#define P6_7 (0x600 | BIT7)

#define P7_0 (0x700 | BIT0)
#define P7_1 (0x700 | BIT1)
#define P7_2 (0x700 | BIT2)
#define P7_3 (0x700 | BIT3)
#define P7_4 (0x700 | BIT4)
#define P7_5 (0x700 | BIT5)
#define P7_6 (0x700 | BIT6)
#define P7_7 (0x700 | BIT7)

#define P8_0 (0x800 | BIT0)
#define P8_1 (0x800 | BIT1)
#define P8_2 (0x800 | BIT2)
#define P8_3 (0x800 | BIT3)
#define P8_4 (0x800 | BIT4)
#define P8_5 (0x800 | BIT5)
#define P8_6 (0x800 | BIT6)
#define P8_7 (0x800 | BIT7)


typedef uint16_t gpio_t;

//

void GPIO_HIGH(gpio_t port) { PxOUT(port) |= _PIN(port); }
void GPIO_LOW(gpio_t port) { PxOUT(port) &= _PIN(port); }
void GPIO_TOGGLE(gpio_t port) { PxOUT(port) ^= _PIN(port); }

void GPIO_setAsInputPinWithPullUpResistor(gpio_t port) {
	PxSEL(port) &= ~_PIN(port);
	PxDIR(port) &= ~_PIN(port);
	PxREN(port) |= _PIN(port);
	PxOUT(port) |= _PIN(port);
}

void GPIO_select_rising_edge(gpio_t port) {
	PxIES(port) &= ~_PIN(port);
}

void GPIO_select_falling_edge(gpio_t port) {
	PxIES(port) |= _PIN(port);
}

void GPIO_clearInterrupt(gpio_t port) {
	PxIFG(port) &= ~_PIN(port);
}

void GPIO_enableInterrupt(gpio_t port) {
	PxIE(port) |= _PIN(port);
}

void GPIO_disableInterrupt(gpio_t port) {
	PxIE(port) &= ~_PIN(port);
}

void GPIO_setAsOutputPin(gpio_t port) {
	PxSEL(port) &= ~ _PIN(port);
	PxDIR(port) |= _PIN(port);
}

void GPIO_set_input(gpio_t port) { GPIO_setAsInputPinWithPullUpResistor(port); GPIO_select_rising_edge(port); GPIO_clearInterrupt(port); GPIO_enableInterrupt(port); }
void GPIO_set_output(gpio_t port) { GPIO_setAsOutputPin(port); GPIO_LOW(port); }

void GPIO_pulse(gpio_t port, uint32_t microseconds) {
	PxOUT(port) |= _PIN(port);
	usleep(microseconds);
	PxOUT(port) &= ~_PIN(port);
}

int GPIO_has_interrupt(gpio_t port) {
	return PxIFG(port) & _PIN(port);
}

bool GPIO_is_rising_egde(gpio_t port) { return !(PxIES(port) & _PIN(port)); }
bool GPIO_is_falling_egde(gpio_t port) { return (PxIES(port) & _PIN(port)); }

// Leds

#define LED1_OFF() P1OUT &= ~BIT0;
#define LED1_ON() P1OUT |= BIT0;
#define LED1_TOGGLE() P1OUT ^= BIT0;

#define LED2_OFF() P4OUT &= ~BIT7;
#define LED2_ON() P4OUT |= BIT7;
#define LED2_TOGGLE() P4OUT ^= BIT7;


#endif /* GPIO_H_ */

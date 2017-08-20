/*
 * time.h
 *
 *  Created on: 17 août 2017
 *      Author: jfellus
 */

#ifndef TIME_H_
#define TIME_H_

#include <msp430.h>


void usleep(uint32_t x) {
	x>>=2;
	while(x--) __delay_cycles(12);
}


static uint32_t milliseconds = 0;

/** Setup Timer A_0 as a millisecond counter */
inline static void timer_init() {
	TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
	TA0CCR0 = 8000;				    // 1ms at 8mhz
	TA0CTL = TASSEL__SMCLK + MC_1;
	milliseconds = 0;
}

/** Reset timer to 0 */
inline static void timer_reset() {
	TA0CTL|=TACLR;   // clears timer A
	TA0R=0;
	milliseconds = 0;
}

inline static uint32_t millis() { return milliseconds; }
inline static uint32_t micros() { return milliseconds*1000 + TA0R; }

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void) { milliseconds++; }

#endif /* TIME_H_ */

/*
 * pwm.h
 *
 *  Created on: 21 août 2017
 *      Author: jfellus
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

// PINOUT
// ----------
// PWM1 = P3.5
// PWM2 = P3.6

#define PWM_CYCLES 200 // = 8MHz/40kHz

void pwm_init() {
	TB0CTL = TASSEL__SMCLK + MC_1;
	TB0CCR0 = PWM_CYCLES;

	TB0CCTL5 = OUTMOD_7;
	TB0CCR5 = 0;

	TB0CCTL6 = OUTMOD_7;
	TB0CCR6 = 0;

	P3SEL |= BIT5 | BIT6; // Set P3.5 and P3.6 as PWM outputs
	P3DIR |= BIT5 | BIT6;
}

/** Set duty for P3.5 PWM */
void pwm1_set_duty(float x) {
	TB0CCR5 = x*PWM_CYCLES;
}

/** Set duty for P3.6 PWM */
void pwm2_set_duty(float x) {
	TB0CCR6 = x*PWM_CYCLES;
}





#endif /* SRC_PWM_H_ */

/*
 * pwm.h
 *
 *  Created on: 21 août 2017
 *      Author: jfellus
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#include <sys/types.h>

// PINOUT
// ----------
// PWM1 = P3.5
// PWM1_DIR = P4.1
// PWM2 = P3.6
// PWM2_DIR = P4.2

#define PWM_CYCLES 200 // = 8MHz/40kHz
#define P_PWM1_DIR P4_1
#define P_PWM2_DIR P4_2


/** Set duty for P3.5 PWM */
void pwm1_set_duty(float x) {
	if(x<0) { x=-x; GPIO_HIGH(P_PWM1_DIR); }
	else GPIO_LOW(P_PWM1_DIR);
	TB0CCR5 = (uint16_t)(x*PWM_CYCLES);
}

/** Set duty for P3.6 PWM */
void pwm2_set_duty(float x) {
	if(x<0) { x=-x; GPIO_HIGH(P_PWM2_DIR); }
	else GPIO_LOW(P_PWM2_DIR);
	TB0CCR6 = (uint16_t)(x*PWM_CYCLES);
}

void pwm_init() {
	GPIO_set_output(P_PWM1_DIR);
	GPIO_set_output(P_PWM2_DIR);

	TB0CTL = TASSEL__SMCLK + MC_1;
	TB0CCR0 = PWM_CYCLES;

	TB0CCTL5 = OUTMOD_7;
	TB0CCR5 = 0;

	TB0CCTL6 = OUTMOD_7;
	TB0CCR6 = 0;

	P3SEL |= BIT5 | BIT6; // Set P3.5 and P3.6 as PWM outputs
	P3DIR |= BIT5 | BIT6;

	pwm1_set_duty(0);
	pwm2_set_duty(0);
}







#endif /* SRC_PWM_H_ */

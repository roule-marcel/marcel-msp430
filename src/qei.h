/*
 * qei.h
 *
 *  Created on: 21 août 2017
 *      Author: jfellus
 */

#ifndef SRC_QEI_H_
#define SRC_QEI_H_

#define P_QEI1A P2_3
#define P_QEI1B P2_6
#define P_QEI2A P2_0
#define P_QEI2B P2_2

#define QEI_STEP 1


uint32_t encoder_left = 0;
uint32_t encoder_right = 0;


void qei_init() {
	GPIO_setAsInputPinWithPullUpResistor(P_QEI1A);
	GPIO_enableInterrupt(P_QEI1A);
	GPIO_select_rising_edge(P_QEI1A);

	GPIO_setAsInputPinWithPullUpResistor(P_QEI1B);
	GPIO_enableInterrupt(P_QEI1B);
	GPIO_select_rising_edge(P_QEI1B);

	GPIO_setAsInputPinWithPullUpResistor(P_QEI2A);
	GPIO_enableInterrupt(P_QEI2A);
	GPIO_select_rising_edge(P_QEI2A);

	GPIO_setAsInputPinWithPullUpResistor(P_QEI2B);
	GPIO_enableInterrupt(P_QEI2B);
	GPIO_select_rising_edge(P_QEI2B);
}


void qei_process_interrupts() {
	if(GPIO_has_interrupt(P_QEI1A)) {
		if(GPIO_is_rising_egde(P_QEI1A)) encoder_left += QEI_STEP;
		else encoder_left -= QEI_STEP;
		GPIO_clearInterrupt(P_QEI1A);
	}
	if(GPIO_has_interrupt(P_QEI1A)) {
		if(GPIO_is_rising_egde(P_QEI1A)) encoder_left += QEI_STEP;
		else encoder_left -= QEI_STEP;
		GPIO_clearInterrupt(P_QEI1A);
	}

	if(GPIO_has_interrupt(P_QEI2A)) {
		if(GPIO_is_rising_egde(P_QEI2A)) encoder_right += QEI_STEP;
		else encoder_right -= QEI_STEP;
		GPIO_clearInterrupt(P_QEI2A);
	}
	if(GPIO_has_interrupt(P_QEI2B)) {
		if(GPIO_is_rising_egde(P_QEI2B)) encoder_right += QEI_STEP;
		else encoder_right -= QEI_STEP;
		GPIO_clearInterrupt(P_QEI2B);
	}
}




#endif /* SRC_QEI_H_ */

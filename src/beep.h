/*
 * beep.h
 *
 *  Created on: 21 août 2017
 *      Author: jfellus
 */

#ifndef SRC_BEEP_H_
#define SRC_BEEP_H_

#define P_BEEP P8_1

void beep_init() {
	GPIO_setAsOutputPin(P_BEEP);
}







#endif /* SRC_BEEP_H_ */

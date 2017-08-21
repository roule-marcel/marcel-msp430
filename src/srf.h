/*
 * srf.h
 *
 *  Created on: 17 août 2017
 *      Author: jfellus
 */

#ifndef SRF_H_
#define SRF_H_

#include "lib/gpio.h"


#define TRIG P6_0


typedef struct {
	gpio_t port;
	uint32_t distance;
	uint32_t t1;
} srf_t;

srf_t SRF[] = {
		{ P1_2, 0, 0 },
		{ P1_3, 0, 0 },
		{ P1_4, 0, 0 },
		{ P1_5, 0, 0 },
		{ P2_4, 0, 0 },
		{ 0,0,0 }
};


void srf_init() {
	GPIO_set_output(TRIG);
	for(int i=0; SRF[i].port; i++) {
		GPIO_set_input(SRF[i].port);
	}
}

void srf_trig() {
	for(int i=0; SRF[i].port; i++) {
		GPIO_disableInterrupt(SRF[i].port);
	}

	GPIO_pulse(TRIG, 20);

	for(int i=0; SRF[i].port; i++) {
		GPIO_clearInterrupt(SRF[i].port);
		GPIO_enableInterrupt(SRF[i].port);
		GPIO_select_rising_edge(SRF[i].port);
	}

	timer_reset();
}


void srf_process_interrupt(srf_t* srf) {
	if(GPIO_has_interrupt(srf->port)) {
		if(GPIO_is_rising_egde(srf->port)) {
			GPIO_select_falling_edge(srf->port);
			srf->t1 = micros();
		}
		else srf->distance = micros();
		GPIO_clearInterrupt(srf->port);
	}
}
void srf_process_interrupts() {
	for(int i=0; SRF[i].port; i++) srf_process_interrupt(&SRF[i]);
}


void srf_print() {
	for(int i=0; SRF[i].port; i++) {
		if(i) uart_putc(' ');
		cprintf("%d", SRF[i].distance);
	}
	uart_putc('\n');
}


#endif /* SRF_H_ */

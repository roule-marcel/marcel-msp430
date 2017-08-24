
#include <msp430.h>

#include "lib/clock.h"
#include "lib/gpio.h"
#include "lib/uartio.h"
#include "lib/time.h"
#include "srf.h"
#include "pwm.h"
#include "qei.h"

int distance;
unsigned long sensor;


void init() {
	GPIO_set_output(P1_0); // LED1
	GPIO_set_output(P4_7); // LED2
	GPIO_set_input(P2_1); // Button 1
	GPIO_set_input(P1_1); // Button 2

	ucs_clockinit(8000000, 1);
	timer_init();
	uart_init();
	srf_init();
	pwm_init();
	qei_init();
}


float x = 0;
float dir = 0.1;

int main(int argc, char *argv[])
{
	init();

	while (1) {
		srf_trig();

		usleep(40000);          // delay for 30ms (after this time echo times out if there is no object detected)

		srf_print();
		qei_print();
		cprintf("%d\n", (int)(x*100));
	}
}


void on_button1();
void on_button2();

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
	srf_process_interrupts();
	if(GPIO_has_interrupt(P1_1))  { on_button2(); GPIO_clearInterrupt(P1_1); }
}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
	srf_process_interrupts();
	qei_process_interrupts();
	if(GPIO_has_interrupt(P2_1)) { on_button1(); GPIO_clearInterrupt(P2_1); }
}





void on_button1() {
	LED1_TOGGLE();
	x+=dir;
	if(x<-1) { dir=-dir; x=-1; }
	if(x>1) { dir=-dir; x=1; }
	pwm1_set_duty(x);
}

void on_button2() {
	LED2_TOGGLE();
	x=0;
	pwm1_set_duty(x);
}

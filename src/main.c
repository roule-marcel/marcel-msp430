
#include <msp430.h>

#include "lib/clock.h"
#include "lib/gpio.h"
#include "lib/uartio.h"
#include "lib/time.h"
#include "srf.h"

int distance;
unsigned long sensor;


void init() {
	GPIO_set_output(P1_0); // LED1
	GPIO_set_output(P4_7); // LED2

	ucs_clockinit(8000000, 1);
	timer_init();
	uart_init();
	srf_init();
}


int main(int argc, char *argv[])
{
	init();

	while (1) {
		srf_trig();

		usleep(40000);          // delay for 30ms (after this time echo times out if there is no object detected)

		for(int i=0; SRF[i].port; i++) {
			if(i) uart_putc(' ');
			cprintf("%d", SRF[i].distance);
		}
		uart_putc('\n');
	}
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) { srf_process_echos(); }
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) { srf_process_echos(); }



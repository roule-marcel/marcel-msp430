/*
 * clock2.h
 *
 *  Created on: 13 août 2017
 *      Author: jfellus
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <msp430.h>
#include <sys/types.h>

void SetVCoreUp (unsigned int level)
{
	// Open PMM registers for write access
	PMMCTL0_H = 0xA5;
	// Set SVS/SVM high side new level
	SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
	// Set SVM low side to new level
	SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
	// Wait till SVM is settled
	while ((PMMIFG & SVSMLDLYIFG) == 0);
	// Clear already set flags
	PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
	// Set VCore to new level
	PMMCTL0_L = PMMCOREV0 * level;
	// Wait till new level reached
	if ((PMMIFG & SVMLIFG))
	while ((PMMIFG & SVMLVLRIFG) == 0);
	// Set SVS/SVM low side to new level
	SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
	// Lock PMM registers for write access
	PMMCTL0_H = 0x00;
}


/** Determines which DCO freq range (DCORSEL) to use (see slas590m.pdf p.31) **/
inline uint16_t _dcorsel_compute_f5529(unsigned long freq) {
	if (freq <= 630000)	return DCORSEL_0;
	if (freq <= 1250000) return DCORSEL_1;
	if (freq <= 2500000) return DCORSEL_2;
	if (freq <= 5000000) return DCORSEL_3;
	if (freq <= 10000000) return DCORSEL_4;
	if (freq <= 20000000) return DCORSEL_5;
	if (freq <= 40000000) return DCORSEL_6;
	if (freq <= 130000000) return DCORSEL_7;
	return 0;
}

/** Enables XT1 as reference clock for FLL */
static void setup_XT1() {
	unsigned long attempts = 0;

#ifdef __MSP430F5172
	PJSEL |= BIT4|BIT5;
#endif
#ifdef __MSP430F5529
	P5SEL |= BIT4|BIT5;  // Select P5.4 and P5.5 for XT1
#endif

	UCSCTL6 &= ~XT1OFF;
	UCSCTL6 = (UCSCTL6 & ~(XCAP_3|XT1DRIVE_3)) | XCAP_0 | XT1DRIVE_3;

	// Wait for XT1 to stabilize
	do {
		UCSCTL7 &= ~XT1LFOFFG;
		attempts++;
	} while ((UCSCTL7 & XT1LFOFFG) && attempts < 1000000);
	if (attempts == 1000000) while(1) { P1OUT ^= BIT0; __delay_cycles(50000); }  // XT1 FAILED

	UCSCTL3 = SELREF__XT1CLK;
}

/** Loop until XT1 & DCO fault flags have cleared **/
static void wait_for_stable_XT1_DCO() {
	P1OUT |= BIT0;
	do {
#ifdef XT1HFOFFG
		UCSCTL7 &= ~(XT1LFOFFG | XT1HFOFFG | DCOFFG);
#else
		UCSCTL7 &= ~(XT1LFOFFG | DCOFFG);
#endif
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG);
	P1OUT &= ~BIT0;
}

void ucs_clockinit(unsigned long freq, uint16_t use_xt1)
{
	WDTCTL = WDTPW + WDTHOLD; // Disable watchdog

	UCSCTL4 = SELM__DCOCLK | SELS__DCOCLK;

	if (use_xt1) setup_XT1();
	else {
		UCSCTL6 |= XT1OFF;
		#ifdef XT1HFOFFG
		UCSCTL7 &= ~(XT1LFOFFG | XT1HFOFFG);
		#else
		UCSCTL7 &= ~XT1LFOFFG;
		#endif
		UCSCTL3 = SELREF__REFOCLK;
	}

	// Set Vcore to maximum
	SetVCoreUp(1);
	SetVCoreUp(2);
	SetVCoreUp(3);

	// Initialize DCO
	__bis_SR_register(SCG0);  // Disable FLL control loop
	UCSCTL0 = 0x0000;

	// Determine which DCORSEL we should use
	UCSCTL1 = DCORSEL_4; //_dcorsel_compute_f5529(freq);

	// FLL reference is 32768Hz, determine multipliers
	uint16_t flln = freq/32768 ;
	uint16_t flld = 0;
	while(flln > 512) { flld++; flln >>= 1; }
	UCSCTL2 = (flld << 12)| (flln-1);

	__bic_SR_register(SCG0);  // Re-enable FLL control loop

	wait_for_stable_XT1_DCO();

	P1IFG = 0;
	_BIS_SR(GIE);                 	    // global interrupt enable
}



#endif /* CLOCK_H_ */

/*
 * uart.h
 *
 *  Created on: 12 août 2017
 *      Author: jfellus
 */

#ifndef UART_H_
#define UART_H_

#include "msp430.h"
#include <sys/types.h>

#define BC_RXBUF_SIZE 256
#define BC_RX_WAKE_THRESH  (1)


// Receive buffer for the UART.  Incoming bytes need a place to go immediately,
// otherwise there might be an overrun when the next comes in.  The USCI ISR
// puts them here.
uint8_t  bcUartRcvBuf[BC_RXBUF_SIZE];

// The index within bcUartRcvBuf, where the next byte will be written.
uint16_t bcUartRcvBufIndex = 0;

// Boolean flag indicating whether bcUartRcvBufIndex has reached the
// threshold BC_RX_WAKE_THRESH.  0 = FALSE, 1 = TRUE
uint8_t  bcUartRxThreshReached = 0;


// Initializes the USCI_A1 module as a UART, using baudrate settings in
// bcUart.h.  The baudrate is dependent on SMCLK speed.
void uart_init(void)
{
    UCA1CTL1 = UCSWRST;        // Put the USCI state machine in reset
    UCA1CTL1 |= UCSSEL__SMCLK;  // Use SMCLK as the bit clock

    UCA1CTL0 = 0; // LSB, 1 stop bit, no parity, 8 bits, clear mode, async

    // Set the baudrate (see http://mspgcc.sourceforge.net/cgi-bin/msp-uart.pl?clock=7995392&baud=9600&submit=calculate)
    UCA1BR0 = 52;
    UCA1BR1 = 0;
    UCA1MCTL = (1 << 4) | (0 << 1) | UCOS16;

    P4SEL |= BIT4+BIT5;         // Configure 4.4 and 4.5 pins as TXD/RXD

    UCA1CTL1 &= ~UCSWRST;       // Take the USCI out of reset

    UCA1IE |= UCRXIE;           // Enable the RX interrupt.  Now, when bytes are
                                // rcv'ed, the USCI_A1 vector will be generated.
}


void uart_putc(uint8_t c) {
    while(!(UCA1IFG & UCTXIFG)) { ; }
    UCA1TXBUF = c;
}

void uart_send(uint8_t * buf, uint8_t len)
{
    uint8_t i = 0;

    // Write each byte in buf to USCI TX buffer, which sends it out
    while (i < len) uart_putc(*(buf+(i++)));
}

// Copies into 'buf' whatever bytes have been received on the UART since the
// last fetch.  Returns the number of bytes copied.
uint16_t bcUartReceiveBytesInBuffer(uint8_t* buf)
{
    uint16_t i, count;

    // Hold off ints for incoming data during the copy
    UCA1IE &= ~UCRXIE;

    for(i=0; i<bcUartRcvBufIndex; i++)
    {
        buf[i] = bcUartRcvBuf[i];
    }

    count = bcUartRcvBufIndex;
    bcUartRcvBufIndex = 0;     // Move index back to the beginning of the buffer
    bcUartRxThreshReached = 0;

    // Restore USCI interrupts, to resume receiving data.
    UCA1IE |= UCRXIE;

    return count;
}



// The USCI_A1 receive interrupt service routine (ISR).  Executes every time a
// byte is received on the back-channel UART.
#pragma vector=USCI_A1_VECTOR
__interrupt void bcUartISR(void)
{
    bcUartRcvBuf[bcUartRcvBufIndex++] = UCA1RXBUF;  // Fetch the byte, store
                                                    // it in the buffer.

    // Wake main, to fetch data from the buffer.
    if(bcUartRcvBufIndex >= BC_RX_WAKE_THRESH)
    {
        bcUartRxThreshReached = 1;
        __bic_SR_register_on_exit(LPM3_bits);       // Exit LPM0-3
    }
}



#endif /* UART_H_ */

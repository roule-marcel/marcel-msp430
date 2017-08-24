# Build & flash

````bash
make
````

# Run

The program communicates with the USB host through backchannel UART on /dev/ttyACM1

````
stty -F /dev/ttyACM1 isig raw 9600; cat /dev/ttyACM1
````

# Used ports


| Port | Function     |
|------|--------------|
| P1.0 | LED1         |
| P1.1 | Button 2     |
| P1.2 | SRF1         |
| P1.3 | SRF2         |
| P1.4 | SRF3         |
| P1.5 | SRF4         |
| P2.1 | Button 1     |
| P2.4 | SRF5         | 
| P3.5 | PWM1         |
| P3.6 | PWM2         |
| P4.1 | DIR1         |
| P4.2 | DIR2         |
| P4.7 | LED2         |
| P8.1 | BEEP         |
| P2.3 | QEI1A        |
| P2.6 | QEI1B        |
| P2.0 | QEI2A        |
| P2.2 | QEI2B        |
| P6.0 | TRIG         |
-----------------------



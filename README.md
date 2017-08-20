# Build & flash

````bash
make
````

# Run

The program communicates with the USB host through backchannel UART on /dev/ttyACM1

````
stty -F /dev/ttyACM1 isig raw 9600; cat /dev/ttyACM1
````



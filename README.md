# ATMega2560 TWI communication 
This project uses two(or more) Arduinos,one configured as master the other ones as slaves and implements an interrupt based interaction protocol on i2c.


The application protocol contains the following messages:
``` 
master -> all: "sample" the slaves sample all digital inputs and puts them in a struct 
master -> slave[i] ->master "get" the slave number sends the sampled digital inputs to the master
master -> slave[i] "set" the master sends to the slave the desired output pin configuration in an internal struct, "without" applying it 
master -> all: "apply" the slaves apply the saved pin status to the outputs
```
The slaves react only to interrupts. they are in halt status. The master offers an interface to the host (PC) program to control the slaves, via UART. All protocols are binary, packet based

# How to use
First of all, install dependencies with:
```
sudo apt-get install arduino arduino-mk
```
Then compile everything using:
```
make
```
This will generate the .elf files for `Master`, `Slave` and the linux executable `Controller.exe`.

To load the program into the master, connect the master to PC and run
```
make Master.hex
```
To load the slave program run
```
make Slave.hex
```

>Note: Slave address is hardcoded. To change it open Slave.c and change the `#define ADDR` directive to whatever address you want to assign to your slave (default is 1)

>Another note: The makefile is configured to work with Atmega2560, some minor tweaks can make it work with other compatible boards too.

# External resources
Some useful resources I used to complete this project. 
* First of all, Atmega [documentation](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf) (Chapter 24)
* This [blog post](http://www.chrisherring.net/all/tutorial-interrupt-driven-twi-interface-for-avr-part1/) by chrisherring
* The [avr user manual](https://www.nongnu.org/avr-libc/user-manual/modules.html)
* Also already posted questions on [avr freaks](https://www.avrfreaks.net) helped

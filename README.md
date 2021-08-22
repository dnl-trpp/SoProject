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

# The protocols
## Master to Slaves Application Protocol (TWI)

 Master Comunicates with slaves using a custom protocol over TWI. The protocol consists of 4 packet types of length 3 bytes:
 ```
----------------------------
| type | payload | checksum |
----------------------------
24     16        8          0
 ```
 Type can be one of 4 types:
 * Type 0x01 :  SAMPLE
 * Type 0x02 :  SET
 * Type 0x03 :  GET
 * Type 0x04 :  APPLY

`SET` and `GET` Packets need to address a specific slave while `SAMPLE` and `APPLY` packets should be sent in broadcast using TWI general call.

Payload is used in `SET` packets and rapresents the pin configuration of slave's `PORTB`

Last byte of data is a checksum, computed simply by adding type and payload togheter.Slave only processes command if the checksum is right.

After sending a `GET` Packet another TWI request needs to be made as Master Receiver and a byte rapresenting the sampled `PORTA` is received.

 >Note: Payload is meaningfull only in the case of `SET` Packets and should be set to `0x00` in other cases

## Pc to Master Proocol (USART)
Master Accepts packets from PCsoftware via uart. Theese packets instruct the Master to send one ofthe above described packets to theslaves. Packets are variablelength 1-3 bytes.
```
---------------------------
| type | address | value  |
---------------------------
```
Types are:
* `m` : in this case no additional byte is needed. It instructs the master to send a `SAMPLE` packet. Master responds with `0xff` in case of success or with a TWI_STATUS byte otherwise.
* `s` : in this case both address and payload are needed. It instructs the master to send a `SET` packet to `address` with `value` as payload. Master responds with `0xff` in case of success or with a TWI_STATUS byte otherwise.
* `g` : in this case just address is needed. It instructs the master to send a `GET` packet to `address` and receive a response. Master responds with `0xff` in case of success or with a TWI_STATUS byte otherwise. In case Master responds with `0xff` another additional byte is sent containing the sampled data.
* `a` : no additional bytes needed. It instructs the master to send an `APPLY` packet. Master responds with `0xff` in case of success or with a TWI_STATUS byte otherwise.

# Usage of Controller Program
This program is an executable file that implements the PC to Master protocol and offers an interface to the user. Executing without arguments connects to `/dev/ttyACM0` otherwise first argument is used as target.

The program accepts the following commands:

* `help` : prints usage
* `set [addr] [value]` : instructs the master to send a `SET` packet to `addr` with `value` as payload. `addr` and `value` can be decimal or hex (prefix with `0x`)
* `get [addr]` : instructs master to get previously sampled values from `addr` and prints the returned data. `addr` can be decimal or hex (prefix with `0x`)
* `sample` : instructs master to send a `SAMPLE` packet
* `apply` : instructs master to send a `APPLY` packet

Here is an example of execution:
```
./Controller.exe
Connecting to /dev/ttyACM0...
[SUCCESS]
> help
Usage:
-- set [addr] [value]
-- get [addr]
-- apply
-- sample
> set 1 0xff
Command sent sucesfully
> apply
Command sent sucesfully
> sample
Command sent sucesfully
> get 1
Command sent sucesfully
Master read d5

```

# Schematics
See Schematics folder



# External resources
Some useful resources I used to complete this project. 
* First of all, Atmega [documentation](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf) (Chapter 24)
* This [blog post](http://www.chrisherring.net/all/tutorial-interrupt-driven-twi-interface-for-avr-part1/) by chrisherring
* The [avr user manual](https://www.nongnu.org/avr-libc/user-manual/modules.html)
* Also already posted questions on [avr freaks](https://www.avrfreaks.net) helped

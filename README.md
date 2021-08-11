# Arduino TWI communication 
Use two Arduinos,one configured as master the other ones as slaves Implement an interrupt based interaction protocol on i2c [http://www.chrisherring.net/all/tutorial-interrupt-driven-twi-interface-for-avr-part1/] 
The protocol should contain the following messages:
``` 
master -> all: "sample" the slaves sample all digital inputs and puts them in a struct 
master -> slave[i] ->master "get" the slave number sends the sampled digital inputs to the master
master -> slave[i] "set" the master sends to the slave the desired output pin configuration in an internal struct, "without" applying it 
master -> all: "apply" the slaves apply the saved pin status to the outputs
```
The slaves react only to interrupts. they are in halt status. The master offers an interface to the host (PC) program to control the slaves, via UART. All protocols are binary, packet based

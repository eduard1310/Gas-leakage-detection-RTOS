# Gas-leakage-detection-RTOS
The project represents an implementation of a monitoring and warning system in case of a gas leaks, this being an IoT(Internet of things) project and use a real time operating 
system (RTOS). 
The first implementation is made with two low power modules, -nrf24l01 wireless, now the communication is with i2c.
The communication is made between two boards, both having inside an ATmega328p micro-controller.
First board having the role of transmitting messages when the gas sensor will detect a high level of gas and the second board is used as a receiver,
receiving the message from the first board and display a message on the LCD and depending on the value from that message.
In the same time, when we will display the warning message on the LCD the buzzer will start to generate a specific sound.

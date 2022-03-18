# QSpiToUSB-F7

Repository that conatain projects, examples and tools for creating a STM32F7 to work as a quad spi slave, receive only, and relay that data over USB to connected PC. Goal is to reach speeds at least over 100Mbps. 

# Structure of repository
The repository is as of right now divided into two parts. One about how to do high speed USB transfer from the F7 to the PC and one how to read quad spi data from gpio register and then combine the nibbles and store it. 

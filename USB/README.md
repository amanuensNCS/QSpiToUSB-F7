#Contents:

## ST_UsbFiles

Conatins the c library used to make an StM32F7 work as a high speed usb device with focus on sending data to the PC.

Using the usb library will setup the F7 to signal to the PC that it is a "Vendor Specific" USB device when plugged in. This means that the PC will not load any drivers automatically but instead we can access the device using a generic usb library and do standard read and write actions on the device.

Documentation of the library is found [here](St_UsbFiles).

##tools

Contains tools for reading and checking the transmitted speed from the F7 to the PC.

## example

Contains a whole STM32CubeIDE project using the library in this repository to set up a F7 to send a 0->255 counter to the PC as fast as possible.


# Content

## ST_UsbFiles

Conatins the c library used to make a StM32F7 work as a high speed usb device with focus on sending data to the PC.

Using the the files in ST_UsbFiles will setup a F7 to be able to send large quantaties of data to the PC when receiving IN requests (USB transfers are allways initiated from the PC side and IN requests refers to going IN to the PC host). The F7 will present itself as a "Vendor Specific" USB device when plugged in. This means that the PC will not load any drivers automatically but instead we can access the device using a generic usb calls and that way easily do standard read and write actions on the device.

Documentation of the files is found [here](St_UsbFiles).

## tools

Contains tools for reading and checking the transmitted speed from the F7 to the PC.

## example

Contains a whole STM32CubeIDE project using the library in this repository to set up a F7 to send a 0->255 counter to the PC as fast as possible.


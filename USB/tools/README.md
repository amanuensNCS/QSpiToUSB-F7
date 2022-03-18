# Tools
Tools for speed testing the QuadSpi Usb device. There is both an Python and a C version. Note however that the Python version only can reach speeds of about 8 MByte/s no matter the performance of the ST chip. On the C version we have recorded speeds of around 28 MByte/s and we guess that the C program is not the bottleneck.

# USB Drivers
Some background info:
When a usb device is connected, the computer asks for some standardised data from the device. One important such data is the "Device Class". The device class string helps the computer to determine what driver it should use to drive the device. 

In our case the ST chip will signal it is a "Vendor Specific Class" which will make the PC OS not load any driver by default. Our way to communicate then is using generic usb calls directly from code. These calls are available from the libusb package and the call we mostly use is the one to read from a generic "IN Endpoint" which is how we get the Quad spi data. 

# How to run
## Python
The python program can be run, on linux, via:
```
sudo python main.py
```
The sudo can be removed if you have change your udev rules to give access to the USB device.

## C
To compile the C program you have to first get the libusb library and then include it while compiling. For me the library lies in /usr/lib/libusb-1.0.so (and include files in /usr/include/libusb-1.0) so I would compile the file via    
```
gcc main.c -L/usr/lib -lusb-1.0 -o QSpiSpeedTest
```
and then run it via
```
sudo ./QSpiSpeedTest
```

# Verifying data
The C program can be made to run to check if the incoming data is an overflowing counter from 0-255. This is done by changing the VERIFY_DATA define and recompiling the project.

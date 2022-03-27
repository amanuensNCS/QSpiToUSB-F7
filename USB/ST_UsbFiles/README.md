# What this library is
This "library" is more of an example use case of ST's USB device library which can be read about in [UM1734](https://www.st.com/resource/en/user_manual/dm00108129-stm32cube-usb-device-library-stmicroelectronics.pdf). When using ST's library it easy to get started in CubeMX by first enabling one of the usb port as usb device and then selecting one of the device classes under middleware. The complication here is that none of the standardized usb classes quite provide the functionality to continously send large amount of data and easily reading this data from code on the pc side.

What we have done to solve this is to modify the existing Communication Device Class (CDC) to be optimized to primerley send data to the pc and changed how it presents itself to the PC to prevent loading any default drivers for it. That way we can easily access get data from PC written code since no other driver is occupying the device. When a device presents itself this way as a non standard device it is called a "Vendor Specfic" device. 

All USB communication goes "endpoints" which usually either are IN or OUT endpoints. In our case we have an "IN" endpoint on address (just a unique identifying number) 0x81 so PC would make an IN request on address 0x81 to get data from the device. How the device sends data to the PC is described below:

## How to use
Enable the high speed usb port in CubeMX and under middleware select the Communcation Device Class. Generate the code. When generating code for high speed USB, CubeMX seems to forget to start the clock to the "ULPI" phy so a "\_\_HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE()" is requeired before "MX_USB_DEVICE_Init()" for it to work properly.

Cube MX should now have generated two folders, "USB_DEVICE" and "Middlewares/ST/STM32_USB_Device_Library". Replace these with the ones given in this repositroy. Description of subfolders:

1. Middlewares/.../Class/CDC/ Contains files for our modified CDC class and all it's callbacks as all of the usb descriptors except the device descriptor (The usb descriptors are what are sent to the PC and tells the PC how to interact with the device).  
2. Middlewares/.../Core/ Contains "core" usb library files and are files we have not modified but are ST's original. 
3. USB_DEVICE/App/ Contains 
  1. Interface functions, API, to the "CDC" class mentioned above in the "*_if.[ch]" files. 
  2. "usbd_desc.[ch]" contains the USB device descriptor 
  3. "usb_device.[ch]" are trivial CubeMX files.
4. USB_DEVICE/Target Contains the layer between the USB library and the F7 hardware specifics which in this case are accessed via HAL. Of special intereset here is the "USBD_LL_Init" function since there you can distribute the total buffer size between the different endpoints. 
  
In code you can now import "usbd_cdc_if.h" and call the "CDC_Trasmit_HS(buffer, length)" which make sure that the data is sent on the next IN requst to address 0x81 from the pc. The library will make sure to alwyas fill the peripheral's USB FIFO buffer when it is halfway empty to prevent the peripheral from having to wait for data. The fifo buffer is 4 kibibyte of which 87% is dedictated to the 0x81 IN endpoint in current configuration. The buffer sizes for each endpoint can be set as described above. 

## Points of intereset
TODO

## Sending data from PC to the F7

Sending data this way is not implemented in these files but if one would want to there would be a few ways to do it.

One way to do it, if the amount of data to receive is small, is to send data using a "Control Transaction"(such a transaction is for instance already used when the PC requests meta data about the device). A control transaction begins with a SETUP packet and the "USBD_CDC_Setup()" callback will be called if it is a "class Request Type" SETUP packet (specified as byte in the packet). So the PC can send begin a control transaction with a setup packet with such a request type and then use an unique "Request" (an other byte in the packet) number which can then be matched agains in the "USBD_CDC_Setup()" callback. 

In the callback it should then be possible to call "USBD_CtlPrepareRx()" to prepare reading data (the control transaction has an optional data stage) which when received should trigger the "USBD_CDC_EP0_RXReady()" callback where the data then can be processed. EP0=Endpoint 0 is the endpoint number of the "control endpoint" so all control transfers will always go to EP0.

The other way to do it would be to open a new endpoint to receive data. This was part of the normal CDC implementation, where you usually communicate both ways, but has been optimized away in this use case. This would enable to use "bulk transfers" which is suitable when sending larger quantities of data.

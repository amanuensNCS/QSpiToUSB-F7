# What this library is
This "library" is more of an example use case of ST's USB device library which can be read about in [UM1734](https://www.st.com/resource/en/user_manual/dm00108129-stm32cube-usb-device-library-stmicroelectronics.pdf). When using ST's library it easy to get started in CubeMX by first enabling one of the usb port as usb device and then selecting one of the device classes under middleware. The complication here is that none of the standardized usb classes quite provide the functionality to continously send large amount of data and providing an easy way to interface on the pc side. 

What we have done to solve this is to modify the existing Communication Device Class (CDC) to be optimized to primerley sending data to the pc and to register iteself as a "Vendor Specific" device class when communicating with the pc.


## Sending data from PC to the F7

Sending data this way is not implemented in these files but if one would want to there would be a few ways to do it.

One way to do it, if the amount of data to receive is small, is to send data using a "Control Transaction"(such a transaction is for instance already used when the PC requests meta data about the device). A control transaction begins with a SETUP packet and the "USBD_CDC_Setup()" callback will be called if it is a "class Request Type" SETUP packet (specified as byte in the packet). So the PC can send begin a control transaction with a setup packet with such a request type and then use an unique "Request" (an other byte in the packet) number which can then be matched agains in the "USBD_CDC_Setup()" callback. 

In the callback it should then be possible to call "USBD_CtlPrepareRx()" to prepare reading data (the control transaction has an optional data stage) which when received should trigger the "USBD_CDC_EP0_RXReady()" callback where the data then can be processed. EP0=Endpoint 0 is the endpoint number of the "control endpoint" so all control transfers will always go to EP0.

The other way to do it would be to open a new endpoint to receive data. This was part of the normal CDC implementation, where you usually communicate both ways, but has been optimized away in this use case. This would enable to use "bulk transfers" which is suitable when sending larger quantities of data.

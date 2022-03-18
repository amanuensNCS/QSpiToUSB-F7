# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import usb.core
import time

ENDPOINT_ADDRESS_EP1_IN = 0x81
vendorId = 0x0483
productId = 0x5740


def main():
    dev = usb.core.find(idVendor=vendorId, idProduct=productId)

    readAmount = 0
    if dev is None:
        raise ValueError('Device not found')

    dev.set_configuration()

    t_end = time.time_ns() + 1e9
    while time.time_ns() < t_end:
        ret = dev.read(ENDPOINT_ADDRESS_EP1_IN, 512, 100)
        readAmount += len(ret)

    print(readAmount)

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/

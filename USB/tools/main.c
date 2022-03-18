#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <time.h>

/*If 1 then data is checked to be an increasing 1 byte number that overflows from 255 to 0*/
#define VERIFY_DATA 0
#define ID_VENDOR 0x0483
#define ID_PRODUCT 0x5740
#define rxBufferSize 100000U
#define oneSecondInNS 1e9L
libusb_context *context = NULL;

int main() {

    uint8_t last_read_data = 0;
    if (VERIFY_DATA)
        printf("Verifying received data\n");
    else printf("Not verifying received data\n");
    libusb_device **devs;

    int r;

    r = libusb_init(&context);

    if (r < 0)
        return r;
    ssize_t cnt = libusb_get_device_list(context, &devs);

    libusb_device *foundDevice = NULL;
    for (int i=0; i < cnt; i++) {
        libusb_device *device = devs[i];
        struct libusb_device_descriptor deviceDescriptor;
        libusb_get_device_descriptor(device, &deviceDescriptor);
        if (deviceDescriptor.idVendor == ID_VENDOR && deviceDescriptor.idProduct == ID_PRODUCT) {
            foundDevice = device;
        }
    }

    if (foundDevice) {
        libusb_device_handle *devHandle;
        int err = libusb_open(foundDevice, &devHandle);
        if (err < 0) {
            perror("Failed to open usb device");
            return err;
        }

        unsigned char rxBuffer[rxBufferSize];
        int totalNrBytesRead = 0;

        struct timespec ts_start, ts_curr;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts_start);
        while (1) {

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts_curr);
            // Break after 1 second has passed.
            if ((ts_curr.tv_sec - ts_start.tv_sec) * oneSecondInNS + (ts_curr.tv_nsec - ts_start.tv_nsec) > oneSecondInNS) {
                break;
            }
            //printf("%ld\n", ts_curr.tv_nsec);
            int nrBytesRead;
            err = libusb_bulk_transfer(devHandle, 0x81, rxBuffer, rxBufferSize, &nrBytesRead, 100);
            totalNrBytesRead += nrBytesRead;

            if (VERIFY_DATA) {
                for (int i = 0; i < nrBytesRead; ++i) {
                    if (rxBuffer[i] != last_read_data++) {
                        fprintf(stderr, "Missed value. Jump from %u to %u\n", last_read_data - 1, rxBuffer[i]);
                        last_read_data = rxBuffer[i];
                    }
                }
            }
        }


        if (err != 0) {
            const char *error = libusb_strerror(err);
            //perror("Bulk transfer failed: %s", error);
            printf("Bulk transfer error: %s \n", error);
        }
        printf("%d", totalNrBytesRead);

    }
    else {
        printf("Device not found");
    }

    return 0;
}

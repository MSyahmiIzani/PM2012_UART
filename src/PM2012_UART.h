#ifndef __PM2012_UART_H__
#define __PM2012_UART_H__

#include "Arduino.h"
#include <stdint.h>
#include <stdbool.h>

#define PM2012_UART_STX_SND 0x11
#define PM2012_UART_STX_RCV 0x16

#define PM2012_UART_CMD_READ_MEASUREMENT_RESULT 0x0B
#define PM2012_UART_CMD_READ_SOFTWARE_VERSION_NUMBER 0x1E
#define PM2012_UART_CMD_READ_SERIAL_NUMBER 0x1F

#define PM2012_UART_POS_STX 0
#define PM2012_UART_POS_LEN 1
#define PM2012_UART_POS_CMD 2
#define PM2012_UART_POS_DAT 3

class PM2012_UART {
public:
    PM2012_UART(Stream *uart = &Serial);

    bool measure(void);
    bool requestSoftwareVersionNumber(void);
    bool requestSerialNumber(void);

    uint32_t getPm1p0Grimm(void);
    uint32_t getPm2p5Grimm(void);
    uint32_t getPm10Grimm(void);
    uint32_t getPm1p0Tsi(void);
    uint32_t getPm2p5Tsi(void);
    uint32_t getPm10Tsi(void);
    uint32_t getPm0p3Micro(void);
    uint32_t getPm0p5Micro(void);
    uint32_t getPm1p0Micro(void);
    uint32_t getPm2p5Micro(void);
    uint32_t getPm5p0Micro(void);
    uint32_t getPm10Micro(void);

    void readSoftwareVersionNumber(char * pData);
    void readSerialNumber(uint8_t * pData);
private:
    void clearBuffer(void);

    int16_t recvData(uint8_t cmd, uint32_t timeout = 1000);

    Stream *uart;
    uint8_t rxBuffer[256];
    uint8_t idxBuffer;
    uint8_t txBuffer[256];
    uint8_t data[256];

    uint32_t pm1p0Grimm;
    uint32_t pm2p5Grimm;
    uint32_t pm10Grimm;
    uint32_t pm1p0Tsi;
    uint32_t pm2p5Tsi;
    uint32_t pm10Tsi;
    uint32_t pm0p3Micro;
    uint32_t pm0p5Micro;
    uint32_t pm1p0Micro;
    uint32_t pm2p5Micro;
    uint32_t pm5p0Micro;
    uint32_t pm10Micro;

    char softwareVersionNumber[13];
    uint8_t serialNumber[10];
};

#endif // __PM2012_UART_H__

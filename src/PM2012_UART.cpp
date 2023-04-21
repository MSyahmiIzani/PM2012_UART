#include "PM2012_UART.h"

PM2012_UART::PM2012_UART(Stream *uart)
    : uart(uart)
{
}

void PM2012_UART::clearBuffer(void)
{
    idxBuffer = 0;
}

int16_t PM2012_UART::recvData(uint8_t cmd, uint32_t timeout)
{
    clearBuffer();
    uint32_t start = millis();

    uint8_t b;

    while (millis() - start < timeout)
    {
        while (uart->available())
        {
            b = uart->read();

            switch (idxBuffer)
            {
            case 0: // STX
            {
                if (b != PM2012_UART_STX_RCV)
                {
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            case 1: // LEN
            {
                if (b < 1)
                {
                    idxBuffer = 0;
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            case 2: // CMD
            {
                if (b != cmd)
                {
                    idxBuffer = 0;
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            default:
            {
                rxBuffer[idxBuffer++] = b;

                uint8_t len = rxBuffer[PM2012_UART_POS_LEN];

                if (idxBuffer >= len + 3)
                {
                    idxBuffer = 0;

                    uint8_t cs = 0;

                    for (uint8_t i = 0; i < len + 2; i++)
                    {
                        cs += rxBuffer[i];
                    }

                    cs = - cs;

                    if (cs != rxBuffer[len + 2])
                    {
                        return -1;
                    }

                    uint8_t c = rxBuffer[PM2012_UART_POS_CMD];

                    uint8_t dataLength = rxBuffer[PM2012_UART_POS_LEN] - 1;

                    if (dataLength == 0)
                    {
                        return dataLength;
                    }

                    for (uint8_t i = 0; i < dataLength; i++)
                    {
                        *(data + i) = rxBuffer[PM2012_UART_POS_DAT + i];
                    }

                    return dataLength;
                }
                break;
            }
            }
        }
    }

    return -1;
}

bool PM2012_UART::measure(void)
{
    txBuffer[0] = PM2012_UART_STX_SND;
    txBuffer[1] = 0x02;
    txBuffer[2] = PM2012_UART_CMD_READ_MEASUREMENT_RESULT;
    txBuffer[3] = 0x07;

    uint8_t sum = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        sum += txBuffer[i];
    }

    // CS
    txBuffer[4] = - sum;

    uart->write(txBuffer, 5);

    int16_t len = recvData(PM2012_UART_CMD_READ_MEASUREMENT_RESULT);

    
    if (len != 52)
    {
        return false;
    }
    

    pm1p0Grimm = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
    pm2p5Grimm = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];
    pm1p0Grimm = (data[8] << 24) + (data[9] << 16) + (data[10] << 8) + data[11];
    pm1p0Tsi = (data[12] << 24) + (data[13] << 16) + (data[14] << 8) + data[15];
    pm2p5Tsi = (data[16] << 24) + (data[17] << 16) + (data[18] << 8) + data[19];
    pm1p0Tsi = (data[20] << 24) + (data[21] << 16) + (data[22] << 8) + data[23];
    pm0p3Micro = (data[24] << 24) + (data[25] << 16) + (data[26] << 8) + data[27];
    pm0p5Micro = (data[28] << 24) + (data[29] << 16) + (data[30] << 8) + data[31];
    pm1p0Micro = (data[32] << 24) + (data[33] << 16) + (data[34] << 8) + data[35];
    pm2p5Micro = (data[36] << 24) + (data[37] << 16) + (data[38] << 8) + data[39];
    pm5p0Micro = (data[40] << 24) + (data[41] << 16) + (data[42] << 8) + data[43];
    pm10Micro = (data[44] << 24) + (data[45] << 16) + (data[46] << 8) + data[47];

    return true;
}

bool PM2012_UART::requestSoftwareVersionNumber(void)
{
    txBuffer[0] = PM2012_UART_STX_SND;
    txBuffer[1] = 0x01;
    txBuffer[2] = PM2012_UART_CMD_READ_SOFTWARE_VERSION_NUMBER;

    uint8_t sum = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        sum += txBuffer[i];
    }

    // CS
    txBuffer[3] = - sum;

    uart->write(txBuffer, 4);

    int16_t len = recvData(PM2012_UART_CMD_READ_SOFTWARE_VERSION_NUMBER);

    if (len != 13)
    {
        return false;
    }

    for (uint8_t i = 0; i < 13; i++)
    {
        softwareVersionNumber[i] = data[i];
    }

    return true;
}

bool PM2012_UART::requestSerialNumber(void)
{
    txBuffer[0] = PM2012_UART_STX_SND;
    txBuffer[1] = 0x01;
    txBuffer[2] = PM2012_UART_CMD_READ_SERIAL_NUMBER;

    uint8_t sum = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        sum += txBuffer[i];
    }

    // CS
    txBuffer[3] = - sum;

    uart->write(txBuffer, 4);

    int16_t len = recvData(PM2012_UART_CMD_READ_SERIAL_NUMBER);

    if (len != 10)
    {
        return false;
    }

    for (uint8_t i = 0; i < 10; i++)
    {
        serialNumber[i] = data[i];
    }

    return true;
}

uint32_t PM2012_UART::getPm1p0Grimm(void)
{
    return pm1p0Grimm;
}

uint32_t PM2012_UART::getPm2p5Grimm(void)
{
    return pm2p5Grimm;
}

uint32_t PM2012_UART::getPm10Grimm(void)
{
    return pm10Grimm;
}

uint32_t PM2012_UART::getPm1p0Tsi(void)
{
    return pm1p0Tsi;
}

uint32_t PM2012_UART::getPm2p5Tsi(void)
{
    return pm2p5Tsi;
}

uint32_t PM2012_UART::getPm10Tsi(void)
{
    return pm10Tsi;
}

uint32_t PM2012_UART::getPm0p3Micro(void)
{
    return pm0p3Micro;
}

uint32_t PM2012_UART::getPm0p5Micro(void)
{
    return pm0p5Micro;
}

uint32_t PM2012_UART::getPm1p0Micro(void)
{
    return pm1p0Micro;
}

uint32_t PM2012_UART::getPm2p5Micro(void)
{
    return pm2p5Micro;
}

uint32_t PM2012_UART::getPm5p0Micro(void)
{
    return pm5p0Micro;
}

uint32_t PM2012_UART::getPm10Micro(void)
{
    return pm10Micro;
}


void PM2012_UART::readSoftwareVersionNumber(char *pData)
{
    for (uint8_t i = 0; i < 13; i++)
    {
        pData[i] = softwareVersionNumber[i];
    }
}

void PM2012_UART::readSerialNumber(uint8_t *pData)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        pData[i] = serialNumber[i];
    }
}

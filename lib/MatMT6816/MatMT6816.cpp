//
// Created by Matieusz on 09.04.2026.
//

#include "MatMT6816.hxx"
#include <SPI.h>

// template <int CS, SPIClass*spi, EncoderRes encoderRes>
// int MatT6816<CS, spi, encoderRes>::init()
// {
//     pinMode(CS, OUTPUT);
//     digitalWrite(CS, HIGH);
//
//     return 1;
// }


template <int CS, SPIClass*spi, EncoderRes encoderRes>
MatMT6816<CS, spi, encoderRes>::MatMT6816()
{
    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
}


template <int CS, SPIClass*spi, EncoderRes encoderRes>
int MatMT6816<CS, spi, encoderRes>::getAngleRaw()
{
    spi->beginTransaction(MT8616Settings);
    digitalWrite(CS, LOW);

    spi->transfer(MT6816_READ_REG_03);
    const uint16_t result = spi->transfer16(0x00);

    digitalWrite(CS, HIGH);
    spi->endTransaction();

    if (result & (0x02))
    {
        return -1;
    }
    return result >> 2;
}


template <int CS, SPIClass* spi, EncoderRes encoderRes>
double MatMT6816<CS, spi, encoderRes>::getSpeed()
{
    uint32_t currentTime = GET_TIME();
    int currentPulses = getAngleRaw();
    if (currentPulses < 0) return 0.0;
    double deltaTime = (currentTime - prevTime) / 1000000.0;

    int deltaPulses = currentPulses - prevPulses;

    int maxVal = (1 << static_cast<int>(encoderRes));
    if (deltaPulses > (maxVal / 2)) deltaPulses -= maxVal;
    else if (deltaPulses < -(maxVal / 2)) deltaPulses += maxVal;
    double rpm = (static_cast<double>(deltaPulses) / maxVal) / deltaTime * 60.0;

    prevPulses = static_cast<uint16_t>(currentPulses);
    prevTime = currentTime;

    return rpm;
}

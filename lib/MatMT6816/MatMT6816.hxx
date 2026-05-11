//
// Created by Matieusz on 09.04.2026.
//


#pragma once
#ifndef LINEFOLLOWER_MATMT6816_HXX
#define LINEFOLLOWER_MATMT6816_HXX

#include "SPI.h"

#define TwoPI 2*3.1415926535897932384626433832795

#define MT6816_READ_REG_03                  0x83
#define MT6816_READ_REG_04                  0x84

#define GET_TIME micros

static SPISettings MT8616Settings(1000000, MSBFIRST, SPI_MODE3);


enum class EncoderRes
{
	B14 = 14,
};


template <int CS, SPIClass *spi = &SPI, EncoderRes encoderRes = EncoderRes::B14>
class MatMT6816
{
public:
	//double readAngle();

	MatMT6816();
	double operator()() { return getAngleRaw(); };

	double readAngleDeg() { toDeg(getAngleRaw()); };

	double readAngleRad() { toRad(getAngleRaw()); };

	double getSpeed();

	// static int init();

	double getSpeed();

private:
	static constexpr double pulseToDeg = 360 / static_cast<double>(1 << static_cast<int>(encoderRes));
	static constexpr double pulseToRad = TwoPI / static_cast<double>(1 << static_cast<int>(encoderRes));

	static inline double toDeg(const int pulses) { return pulses * pulseToDeg; };

	static inline double toRad(const int pulses) { return pulses * pulseToRad; };

	static int getAngleRaw();

	double speed = 0;
	uint16_t prevPulses = 0;
	uint32_t prevTime = 0;

	bool canReadAngle = true;
};

#endif //LINEFOLLOWER_MATMT6816_HXX

//
// Created by Matieusz on 09.04.2026.
//


#pragma once
#ifndef LINEFOLLOWER_MATMT6816_HXX
#define LINEFOLLOWER_MATMT6816_HXX


#define TwoPI 3.1415926535897932384626433832795

enum class EncoderMode {
	B16 = 16,
	B14 = 14,
	B12 = 2
};

template<int CS, int MISO, int MOSI, int SCK, int noOfBits>
class MatT6816 {
public:
	//double readAngle();

	double operator()() { return __getAngle(); };

	double readAngleDeg();

	double readAngleRad();

	double getSpeed();

	bool init();

	double getSpeed();

private:
	static constexpr double pulseToDeg = static_cast<double>(2 << noOfBits) / 360;
	static constexpr double pulseToRad = static_cast<double>(2 << noOfBits) / TwoPI;

	inline double toDeg(int pulses);

	inline double toRad(int pulses);

	double __getAngle();

	double speed        = 0;
	bool   canReadAngle = true;
};

#endif //LINEFOLLOWER_MATMT6816_HXX

//
// Created by Matieusz on 04.05.2026.
//

#ifndef LINEFOLLOWER_MAT_VOLTAGE_SENSOR_HXX
#define LINEFOLLOWER_MAT_VOLTAGE_SENSOR_HXX

#include <Arduino.h>

template <int pin, int num, int denom, int maxMiliVolts = 330, int resolution = 1024>
class MatVoltageSensor
{
public:
	static int getRaw() { return analogRead(pin); };
	static double getVoltage() { return (analogRead(pin) * multiplier); }

private:
	static constexpr double multiplier = ((maxMiliVolts / 100.0) / resolution) * (num / static_cast<double>(denom));
};


#endif //LINEFOLLOWER_MAT_VOLTAGE_SENSOR_HXX

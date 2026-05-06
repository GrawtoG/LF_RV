//
// Created by Matieusz on 04.05.2026.
//

#ifndef LINEFOLLOWER_MAT_VOLTAGE_SENSOR_HXX
#define LINEFOLLOWER_MAT_VOLTAGE_SENSOR_HXX

#include <Arduino.h>

template<int pin, int num, int denom>
class MatVoltageSensor {
public:
	double
	double getVoltage() { return analogRead(pin); };

private:
	const double ratio = num / static_cast<double>(denom);
};


#endif //LINEFOLLOWER_MAT_VOLTAGE_SENSOR_HXX

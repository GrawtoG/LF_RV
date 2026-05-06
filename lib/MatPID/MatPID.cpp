//
// Created by Matieusz on 06.04.2026.
//

#include "MatPID.hxx"
#include <Arduino.h>

MatPID::MatPID(double _Kp, double _Ki, double _Kd, const DerivativeMode mode) {
	derivativeMode = mode;
	switch (derivativeMode) {
		case DerivativeMode::OnError:
			derivativeFunction = DerivativeOnError;
			break;
		case DerivativeMode::OnMeasurement:
			derivativeFunction = DerivativeOnMeasurement;
			break;
		default:
			derivativeFunction = DerivativeOnMeasurement;
			break;
	}
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
}


double MatPID::DerivativeOnError(const MatPID *self) {
	return self->_error - self->lastError;
}

double MatPID::DerivativeOnMeasurement(const MatPID *self) {
	return -(self->_input - self->lastInput);
}

double MatPID::operator()(double setpoint, double input, double dt) {
	_error = setpoint - input;
	_input = input;

	iTerm += Ki * _error * dt;

	if (iTerm > OutputClampMax) {
		iTerm = OutputClampMax;
	} else if (iTerm < OutputClampMin) {
		iTerm = OutputClampMin;
	}
	double output = Kp * _error + iTerm + Kd * derivativeFunction(this) / dt;

	if (output > OutputClampMax) {
		output = OutputClampMax;
	} else if (output < OutputClampMin) {
		output = OutputClampMin;
	}

	// http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-reset-windup/
	// from comments

	//
	// if (output > OutputClampMax) {
	// 	iTerm  -= output - OutputClampMax;
	// 	output = OutputClampMax;
	// } else if (output < OutputClampMin) {
	// 	iTerm  += OutputClampMin - output;
	// 	output = OutputClampMin;
	// }

	lastInput = input;
	lastError = _error;
	return output;
}

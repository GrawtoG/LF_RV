//
// Created by Matieusz on 06.04.2026.
//
#pragma once

#ifndef MATPID_HXX
#include "float.h"

class MatPID;

enum class RegulatorType {
	PID,
	BangBang,
};

struct RegConfig {
	//PID
	double Kp = 1;
	double Ki = 0;
	double Kd = 0;
	//

	double OutputClam x              = 1;
	double            OutputClampMin = -1;

	double iTermMax = 1;
	double iTermMin = -1;

	//feedforwardfunc
	double (*ff_func)(double) = [](double x) { return x; };

	//BangBang
	double minOutput  = -1.0;
	double maxOutput  = 1.0;
	double hysteresis = 0.1;

private:
	RegulatorType regulatorType = regulatorType::PID;
};

enum class DerivativeMode {
	OnError,
	OnMeasurement,
};


// One could also use a member function pointer
// using DerivativeFunc = double(MatPID::*)();
using DerivativeFunc = double(*)(const MatPID *);
using RegulatorFunc  = double(MatPID::*)(double, double, double);


class MatPID {
public:
	MatPID();

	MatPID(double _Kp, double _Ki, double _Kd, DerivativeMode mode);


	void SetDerivativeMode(DerivativeMode mode) { derivativeMode = mode; };

	void SetKParameters(double _Kp, double _Ki, double _Kd) {
		Kp = _Kp;
		Ki = _Ki;
		Kd = _Kd;
	};

	void SetKp(double _Kp) {
		Kp = _Kp;
	};

	void SetKi(double _Ki) {
		Ki = _Ki;
	};

	void SetKd(double _Kd) {
		Kd = _Kd;
	};

	void SetOutputLimits(double _min, double _max) {
		OutputClampMin = _min;
		OutputClampMax = _max;
	};

	double operator()(double setPoint, double input, double dt);

private:
	DerivativeMode derivativeMode;
	DerivativeFunc derivativeFunction;
	RegConfig      regConfig;


	double iTerm = 0;

	double lastError  = 0;
	double lastInput  = 0;
	double lastOutput = 0;

	double _error = 0;
	double _input = 0;

	static double DerivativeOnError(const MatPID *self);

	static double DerivativeOnMeasurement(const MatPID *self);


	double BangBang_Compute()
};


#define MATPID_HXX

#endif //MATPID_HXX

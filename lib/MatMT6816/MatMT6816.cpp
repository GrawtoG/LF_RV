//
// Created by Matieusz on 09.04.2026.
//

#include "MatMT6816.hxx"


bool MatT6816::init() {
	return false;
}

double MatT6816::operator ()() {
	return __getAngle();
}


double MatT6816::toDeg(int pulses) {
	return pulses * pulseToDeg;
}

double MatT6816::toRad(int pulses) {
	return pulses * pulseToRad;
}

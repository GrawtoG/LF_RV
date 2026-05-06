//
// Created by Matieusz on 07.04.2026.
//

#include "MatIIR.hxx"


double MatIIR::operator ()(double x) {
	return lastOutput = alpha * x + (oneMinusAlpha) * lastOutput;
}

//
// Created by Matieusz on 07.04.2026.
//

#ifndef LINEFOLLOWER_MATIIR_HXX
#define LINEFOLLOWER_MATIIR_HXX

class MatIIR {
public:
	MatIIR();

	MatIIR(double alpha, double lastOutput = 0) : alpha(alpha), oneMinusAlpha(1 - alpha), lastOutput(lastOutput) {
	};
	double operator()(double x);

	void SetAlpha(double _alpha) {
		alpha         = _alpha;
		oneMinusAlpha = 1 - alpha;
	};



private:
	double alpha;
	double oneMinusAlpha;
	double lastOutput = 0;
};
#endif //LINEFOLLOWER_MATIIR_HXX

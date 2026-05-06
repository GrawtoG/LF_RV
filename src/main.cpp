#include <Arduino.h>
#include <PIO_DShot.h>
#include <MatPID.hxx>
#include <Timers.hpp>

#include "../lib/MatIIR/MatIIR.hxx"


#define RIGHT_WHEEL_PIN 11
#define LEFT_WHEEL_PIN 10


#define LEFT_MOTOR_POLES 12
#define RIGHT_MOTOR_POLES 12

BidirDShotX1 *leftWheelESC;
BidirDShotX1 *rightWheelESC;

MatPID leftWheelPID(1, 1, 0, DerivativeMode::OnMeasurement);
MatPID rightWheelPID(1, 1, 0, DerivativeMode::OnMeasurement);


double targetRPSLeft  = 0;
double targetRPSRight = 0;

double rpsLeft  = 0;
double rpsRight = 0;

MatIIR leftRPSIIR(0.5);
MatIIR rightRPSIIR(0.5);

double leftPID  = 0;
double rightPID = 0;


Timers<4> timers;
Timers<4> timers1;


void blinkLED();

void updateRPS();

void printStuff();

void steerWheels();

void updatePID();


void setup1() {
	Serial.begin(115200);
	Serial.setTimeout(10);
	timers1.attach(0, printStuff, 1000000);
	timers1.attach(1, blinkLED, 1000000);
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	delay(7000); // wait for serial monitor to open
	Serial.println("TL RPS\tRL OUT\tRPS\tTR RPS\tRR OUT\tR RPS");
	leftWheelESC  = new BidirDShotX1(LEFT_WHEEL_PIN);
	rightWheelESC = new BidirDShotX1(RIGHT_WHEEL_PIN);

	leftWheelPID.SetOutputLimits(0, 2000);
	rightWheelPID.SetOutputLimits(0, 2000);
	timers.attach(0, updateRPS, 200);
	timers.attach(1, steerWheels, 200);
	timers.attach(2, updatePID, 20000);
}

void loop1() {
	timers1.proceed();
}

void loop() {
	timers.proceed();

	if (Serial.available()) {
		char cmd = Serial.read();

		String sLeft  = "";
		String sRight = "";

		sLeft  = Serial.readStringUntil(' ');
		sRight = Serial.readString();

		switch (cmd) {
			case 'T':
				targetRPSLeft = sLeft.toDouble();
				targetRPSRight = sRight.toDouble();
				break;
			case 'C':

				leftWheelESC->sendSpecialCommand(sLeft.toInt());
				rightWheelESC->sendSpecialCommand(sRight.toInt());
				break;
			case 'E':
				leftWheelESC->sendSpecialCommand(DSHOT_CMD_EXTENDED_TELEMETRY_ENABLE);
				rightWheelESC->sendSpecialCommand(DSHOT_CMD_EXTENDED_TELEMETRY_ENABLE);
				break;

			case 'P':
				leftWheelPID.SetKp(sLeft.toDouble());
				rightWheelPID.SetKp(sRight.toDouble());
				break;

			case 'I':
				leftWheelPID.SetKi(sLeft.toDouble());
				rightWheelPID.SetKi(sRight.toDouble());
				break;
			case 'D':
				leftWheelPID.SetKd(sLeft.toDouble());
				rightWheelPID.SetKd(sRight.toDouble());
				break;
			case 'A':
				leftRPSIIR.SetAlpha(sLeft.toDouble());
				rightRPSIIR.SetAlpha(sRight.toDouble());
			default:
				targetRPSLeft = 0;
				targetRPSRight = 0;
				break;
		}
	}
}

void steerWheels() {
	leftWheelESC->sendThrottle(leftPID);
	rightWheelESC->sendThrottle(rightPID);
}

void updatePID() {
	static long long lastTime = 0;
	double           dt       = (double) (millis() - lastTime) / (double) 1000;
	leftPID                   = leftWheelPID(targetRPSLeft, rpsLeft, dt);
	rightPID                  = rightWheelPID(targetRPSRight, rpsRight, dt);
	lastTime                  = millis();
}

void updateRPS() {
	uint32_t rpm;

	leftWheelESC->getTelemetryErpm(&rpm);
	rpsLeft = leftRPSIIR((double) rpm / ((LEFT_MOTOR_POLES / 2) * 60.0));

	rightWheelESC->getTelemetryErpm(&rpm);
	rpsRight = rightRPSIIR((double) rpm / ((RIGHT_MOTOR_POLES / 2) * 60.0));
}

void printStuff() {
	Serial.print("targetRPSLeft:");
	Serial.print(targetRPSLeft);
	//Serial.print("\t");

	Serial.print(",leftPID:");
	Serial.print(leftPID);
	//Serial.print("\t");

	Serial.print(",rpsLeft:");
	Serial.print(rpsLeft);
	//Serial.print("\t");

	Serial.print(",targetRPSRight:");
	Serial.print(targetRPSRight);
	//Serial.print("\t");

	Serial.print(",rightPID:");
	Serial.print(rightPID);
	//Serial.print("\t");

	Serial.print(",rpsRight:");
	Serial.println(rpsRight);
}

void blinkLED() {
	static bool state = 0;

	state = !state;

	digitalWrite(LED_BUILTIN, state);
	//PINB = (1 << LED_BUILTIN);
}

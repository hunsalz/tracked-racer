#include "MotorShieldDriver.h"

MotorShieldDriver::MotorShieldDriver(bool enabled,
	unsigned int pinPWM_A,
	unsigned int pinDirA,
	unsigned int pinPWM_B,
	unsigned int pinDirB) : Driver(enabled) {

	_pinPWM_A = pinPWM_A;
	_pinDirA = pinDirA;
	_pinPWM_B = pinPWM_B;
	_pinDirB = pinDirB;

	analogWriteRange(PMW_RANGE); // adjust PMW range
	pinMode(_pinPWM_A, OUTPUT);
	pinMode(_pinDirA, OUTPUT);
	pinMode(_pinPWM_B, OUTPUT);
	pinMode(_pinDirB, OUTPUT);

  Log.verbose(F("Setup motor shield done : PWM(A) = %d with direction = %d, PWM(B) = %d with direction = %d" CR), pinPWM_A, pinDirA, pinPWM_B, pinDirB);
}

int MotorShieldDriver::getSpeedA() {
	return analogRead(_pinPWM_A);
}

int MotorShieldDriver::getSpeedB() {
	return analogRead(_pinPWM_B);
}

int MotorShieldDriver::getDirectionA() {
	return digitalRead(_pinDirA);
}

int MotorShieldDriver::getDirectionB() {
	return digitalRead(_pinDirB);
}

void MotorShieldDriver::setSpeedA(int speed) {

	if (isEnabled()) {
		setSpeed(speed, _pinPWM_A, _pinDirA);
		Log.verbose(F("Applied speed of motor(A) to %d with direction to %d" CR), getSpeedA(), getDirectionA());
	}
}

void MotorShieldDriver::setSpeedB(int speed) {

	if (isEnabled()) {
		setSpeed(speed, _pinPWM_B, _pinDirB);
		Log.verbose(F("Applied speed of motor(B) to %d with direction to %d" CR), getSpeedB(), getDirectionB());
	}
}

void MotorShieldDriver::applySpeedA(int speed) {
  setSpeedA(getSpeedA() + speed);
}

void MotorShieldDriver::applySpeedB(int speed) {
  setSpeedB(getSpeedB() + speed);
}

void MotorShieldDriver::setSpeed(int speed, unsigned int pinPWM, unsigned int pinDir) {

  // limit speed to max. PMW_RANGE
  int _speed = abs(speed);
  if (_speed > PMW_RANGE) {
    _speed = PMW_RANGE;
  }
  // write speed to PWM
  analogWrite(pinPWM, _speed);
  // change direction accordingly to original signed speed to HIGH or LOW
  digitalWrite(pinDir, speed > 0 ? HIGH : LOW);
}

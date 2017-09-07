#include "MotorShieldDriver.h"

MotorShieldDriver::MotorShieldDriver(bool enabled, unsigned int pinPWM, unsigned int pinDir) : MotorDriver(enabled) {

	_pinPWM = pinPWM;
	_pinDir = pinDir;
	_currentSpeed = 0;

	analogWriteRange(PMW_RANGE); // adjust PMW range
	pinMode(_pinPWM, OUTPUT);
	pinMode(_pinDir, OUTPUT);

  Log.verbose(F("Configuration for motor shield done : pwm = %d, direction = %d" CR), pinPWM, pinDir);
}

int MotorShieldDriver::getSpeed() const {
	return _currentSpeed;
}

void MotorShieldDriver::setSpeed(int speed) {
	// control motors if driver is enabled
	if (isEnabled()) {
     // limit speed to max. PMW_RANGE
     int _speed = abs(speed);
     if (_speed > PMW_RANGE) {
       _speed = PMW_RANGE;
     }
     // write speed to PWM
     analogWrite(_pinPWM, _speed);
     // change direction accordingly to original signed speed to HIGH or LOW
     digitalWrite(_pinDir, speed > 0 ? HIGH : LOW);
     // save the current speed setting
     _currentSpeed = _speed;
     Log.verbose(F("Set motor speed to %d and direction to %d" CR), getSpeed(), speed > 0 ? HIGH : LOW);
	}
}

void MotorShieldDriver::apply(int speed) {
  setSpeed(_currentSpeed+speed);
}

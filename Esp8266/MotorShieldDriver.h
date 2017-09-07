#pragma once

#include "MotorDriver.h"

class MotorShieldDriver : public MotorDriver {

	public:

		MotorShieldDriver(bool enabled, unsigned int pinPWM, unsigned int pinDir);

		int getSpeed() const;

		void setSpeed(int speed);

    void apply(int speed);

	private:

		unsigned int _pinPWM;
		unsigned int _pinDir;

		// keeps track of the current speed of the motor driver
		int _currentSpeed = 0;
};

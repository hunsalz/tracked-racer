#pragma once

#include "Driver.h"

class MotorShieldDriver : public Driver {

	public:

		MotorShieldDriver(bool enabled,
			unsigned int pinPWM_A,
			unsigned int pinDirA,
			unsigned int pinPWM_B,
			unsigned int pinDirB);

		int getSpeedA();

		int getSpeedB();

		int getDirectionA();

		int getDirectionB();

		void setSpeedA(int speed);

		void setSpeedB(int speed);

    void applySpeedA(int speed);

		void applySpeedB(int speed);

	private:

		unsigned int _pinPWM_A;
		unsigned int _pinPWM_B;
		unsigned int _pinDirA;
		unsigned int _pinDirB;
    
    void setSpeed(int speed, unsigned int pinPWM, unsigned int pinDir);
};

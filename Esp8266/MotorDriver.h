#pragma once

#include "Driver.h"

class MotorDriver : public Driver {

	public:

		MotorDriver(bool enabled) : Driver(enabled) {}

		// returns current motor speed
		virtual int getSpeed() const = 0;

		// set absolute motor speed
		// speed value = 0 and the motor stops
		// speed value between 1 and and max. PMW_RANGE and the motor runs forward
		// speed value between min. PMW_RANGE and -1 and the motor runs backwards
		virtual void setSpeed(int speed) = 0;

    // applies a relative value to the current motor speed
    // speed value is between min. PMW_RANGE and max. PMW_RANGE
    virtual void apply(int speed) = 0;
};

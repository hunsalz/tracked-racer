#pragma once

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include "Configuration.h"

class Service {

	public:

		// returns if this service is running
		virtual bool isRunning();

		// starts this service and returns true if service is running
		virtual bool start();

    // stops this service and returns true if service is stopped
    virtual bool stop();
};

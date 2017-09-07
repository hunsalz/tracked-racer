#pragma once

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include "Configuration.h"

class Driver {

	public:
		// enabled: enables/disables this driver
		Driver(bool enabled);

		// use this method to know if this driver is enabled or not
		bool isEnabled();

		// enables/disables this driver
		void setEnabled(bool enabled);

	private:

		bool _enabled = false;
};

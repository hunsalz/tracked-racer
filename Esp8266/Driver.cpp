#include "Driver.h"

Driver::Driver(bool enabled) {
	_enabled = enabled;
}

bool Driver::isEnabled() {
	return _enabled;
}

void Driver::setEnabled(bool enabled) {
	_enabled = enabled;
}

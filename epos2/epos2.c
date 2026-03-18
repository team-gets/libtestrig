#include <stdlib.h>
#include <string.h>

#include "definitions.h"
#include "epos2.h"

int AcquireDeviceInformation(struct Controller* controller, int flags) {
	unsigned int error_code = 0;
	int success;
	int selection_end;
	
	if (flags & (1 << 2)) { memset(controller, 0, sizeof(*controller)); }

	if (flags & (1 << 1)) {
		strncpy(controller->Name, "EPOS2", 6);
		strncpy(controller->Protocol, "CANopen", 8);
	}

	// TODO: will this behave expectedly?
	// i.e only one connection to host pc means only one controller shows up?
	// or all three will show
	success = VCS_GetInterfaceNameSelection(controller->Name, controller->Protocol, 1,
			controller->Interface, 64, &selection_end, &error_code);

	return (success == 0) ? 0 : error_code;
}

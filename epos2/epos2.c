#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "definitions.h"
#include "epos2.h"

unsigned int InitializeDevice(struct Controller* controller, void* device_handle) {
	unsigned int error_code;
	int ret;

	device_handle = VCS_OpenDevice(controller->Name,
				controller->Protocol,
				controller->Interface,
				controller->Port,
				&error_code);

	if (device_handle == 0 && error_code != 0) {
		printf("ERROR %uix: Failed to open device with with following characteristics:\n", error_code);
		printf("- Name:      %s\n", controller->Name);
		printf("- Protocol:	 %s\n", controller->Protocol);
		printf("- Interface: %s\n", controller->Interface);
		printf("- Interface: %s\n", controller->Interface);
		return error_code;
	};

	ret = VCS_ClearFault(device_handle, 0, &error_code);

	return (ret == 0 && error_code == 0) ? 0 : error_code;
}

unsigned int AcquireDeviceInformation(struct Controller* controller_out, int flags) {
	unsigned int error_code = 0;
	int success;
	int selection_end;
	
	if (flags & (1 << 2)) { memset(controller_out, 0, sizeof(*controller_out)); }

	if (flags & (1 << 1)) {
		strncpy(controller_out->Name, "EPOS2", 6);
		strncpy(controller_out->Protocol, "CANopen", 8);
	}

	// TODO: will this behave expectedly?
	// i.e only one connection to host pc means only one controller_out shows up?
	// or all three will show
	success = VCS_GetInterfaceNameSelection(controller_out->Name, controller_out->Protocol, 1,
			controller_out->Interface, 64, &selection_end, &error_code);

	return (success == 0 && error_code == 0) ? 0 : error_code;
}

unsigned int CloseDevice(struct Controller* controller, void* device_handle) {
	unsigned int error_code;
	int ret;

	ret = VCS_CloseDevice(device_handle, &error_code);

	return (ret == 0 && error_code == 0) ? 0 : error_code;
}

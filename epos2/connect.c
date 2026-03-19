#include <stdio.h>

#include "definitions.h"
#include "connect.h"

unsigned int InitializeDevice(struct Controller* controller, void* device_handle) {
	unsigned int error_code;
	int ret;

	device_handle = VCS_OpenDevice(controller->Name,
				controller->Protocol,
				controller->Interface,
				controller->Port,
				&error_code);

	if (device_handle == 0 && error_code != 0) {
		printf("ERROR %uix: Failed to open device with with following characteristics:\n",
				error_code);
		return error_code;
	};

	ret = VCS_ClearFault(device_handle, controller->NodeId, &error_code);

	return (ret == 0 && error_code == 0) ? 0 : error_code;
} // uint Open

unsigned int CloseDevice(struct Controller* controller, void* device_handle) {
	unsigned int error_code;
	int ret;

	ret = VCS_CloseDevice(device_handle, &error_code);

	return (ret == 0 && error_code == 0) ? 0 : error_code;
}


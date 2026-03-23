#include <stdio.h>

#include "connect.h"
#include "definitions.h"
#include "identify.h"

static void FailedOpenDevice(uint32_t error_code) {
	printf("ERROR 0x%X: Failed to open device with with following characteristics:\n", error_code);
}

uint32_t InitializeDevices(struct Controller controllers_out[], void* node_1, void* node_2, void* node_3) {
	uint32_t error_code = 0;

	node_1 = VCS_OpenDevice(controllers_out[0].Name,
			controllers_out[0].Protocol,
			controllers_out[0].Interface,
			controllers_out[0].Port,
			&error_code);

	controllers_out[0].NodeId = 1;

	if (node_1 == 0 || error_code != 0) {
		FailedOpenDevice(error_code);
		PrintControllerCharacteristics(&controllers_out[0]);
		//return error_code;
	};

	CleanEnableDevice(&controllers_out[0], node_1);

	node_2 = VCS_OpenSubDevice(node_1,
			controllers_out[1].Name,
			controllers_out[1].Protocol,
			&error_code);

	controllers_out[1].NodeId = 2;

	if (node_2 == 0 || error_code != 0) {
		FailedOpenDevice(error_code);
		PrintControllerCharacteristics(&controllers_out[1]);
		return error_code;
	};

	CleanEnableDevice(&controllers_out[1], node_2);

	node_3 = VCS_OpenSubDevice(node_2,
			controllers_out[2].Name,
			controllers_out[2].Protocol,
			&error_code);

	if (node_3 == 0 || error_code != 0) {
		FailedOpenDevice(error_code);
		PrintControllerCharacteristics(&controllers_out[2]);
		return error_code;
	};

	controllers_out[2].NodeId = 3;

	CleanEnableDevice(&controllers_out[2], node_2);

	return 0;
} // uint Open

uint32_t CleanEnableDevice(struct Controller* controller, void* device_handle) {
	uint32_t error_code = 0;
	int ret;

	ret = VCS_ClearFault(device_handle, controller->NodeId, &error_code);
	if (ret == 0) { return error_code; }

	ret = VCS_SetEnableState(device_handle, controller->NodeId, &error_code);
	controller->State = CTRL_STATE_OPENED;
	return error_code;
}


uint32_t CloseDevice(struct Controller* controller, void* device_handle) {
	uint32_t error_code = 0;
	int ret;

	ret = VCS_CloseDevice(device_handle, &error_code);
	if (ret == 0) { return error_code; }

	ret = VCS_SetDisableState(device_handle, controller->NodeId, &error_code);
	controller->State = CTRL_STATE_CLOSED;
	return error_code;
}


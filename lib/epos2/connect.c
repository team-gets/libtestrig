#include <stdio.h>
#include <assert.h>

#include "epos2.h"
#include "connect.h"
#include "definitions.h"
#include "identify.h"

static void FailedOpenDevice(uint32_t error_code) {
	PrintError(error_code);
	printf("Failed to open device with with following characteristics:\n");
}

uint32_t InitializeDevice(struct Controller* controller_out, void* node, uint8_t node_id) {
	uint32_t error_code = 0;

	node = VCS_OpenDevice(controller_out->Name,
			controller_out->Protocol,
			controller_out->Interface,
			controller_out->Port,
			&error_code);

	controller_out->NodeId = node_id;

	if (node == 0 || error_code != 0) {
		FailedOpenDevice(error_code);
		PrintControllerCharacteristics(controller_out);
	}
	
	CleanEnableDevice(controller_out, node);

	return error_code;
}

uint32_t InitializeDevices(struct Controller controllers_out[], void* handles_out[], uint8_t num) {
	uint32_t error_code = 0;
	assert(num > 1);

	handles_out[0] = VCS_OpenDevice(controllers_out[0].Name,
			controllers_out[0].Protocol,
			controllers_out[0].Interface,
			controllers_out[0].Port,
			&error_code);

	controllers_out[0].NodeId = 1;

	if (handles_out[0] == 0 || error_code != 0) {
		FailedOpenDevice(error_code);
		PrintControllerCharacteristics(&controllers_out[0]);
		return error_code;
	}

	CleanEnableDevice(&controllers_out[0], handles_out[0]);

	for (uint8_t i = 1; i < num; i++) {
		if (handles_out[i] == NULL) { break; }

		handles_out[i] = VCS_OpenSubDevice(handles_out[i],
				controllers_out[i].Name,
				controllers_out[i].Protocol,
				&error_code);

		controllers_out[i].NodeId = i + 1;

		if (handles_out[i] == 0 || error_code != 0) {
			FailedOpenDevice(error_code);
			PrintControllerCharacteristics(&controllers_out[i]);
			return error_code;
		}
	}

	return 0;
} // uint32_t InitializeThreeDevices

uint32_t CleanEnableDevice(struct Controller* controller, void* device_handle) {
	if (device_handle == 0) { return 0x2000000B; }
	uint32_t error_code = 0;
	int ret;

	ret = VCS_ClearFault(device_handle, controller->NodeId, &error_code);
	if (ret == 0) { return error_code; }

	ret = VCS_SetEnableState(device_handle, controller->NodeId, &error_code);
	controller->State = CTRL_STATE_OPENED;
	return error_code;
}

uint32_t CloseDevice(struct Controller* controller, void* device_handle) {
	if (device_handle == 0) { return 0x2000000B; }
	uint32_t error_code = 0;
	int ret;

	ret = VCS_CloseDevice(device_handle, &error_code);
	if (ret == 0) { return error_code; }

	ret = VCS_SetDisableState(device_handle, controller->NodeId, &error_code);
	controller->State = CTRL_STATE_CLOSED;
	return error_code;
}


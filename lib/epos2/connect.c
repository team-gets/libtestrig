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
		return error_code;
	}
	
	error_code = CleanEnableDevice(controller_out, node);
	return error_code;
}

uint32_t InitializeDevices(struct Controller controllers_out[], void* handles_out[], uint8_t num) {
	if (num < 1) { printf("Invalid device array size.\n"); return 0x06040043; } // "General Parameter Error"
	uint32_t error_code = 0;

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

	error_code = CleanEnableDevice(&controllers_out[0], handles_out[0]);
	if (error_code != 0) {
		printf("While attempting to open gateway:\n\t");
		PrintError(error_code);

		return error_code;
	}

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

		error_code = CleanEnableDevice(&controllers_out[i], handles_out[i]);
		if (error_code != 0) {
			printf("While attempting to open gateway:\n\t");
			PrintError(error_code);
			return error_code;
		}
	}

	return 0;
} // uint32_t InitializeThreeDevices

uint32_t CleanEnableDevice(struct Controller* controller, void* device_handle) {
	if (device_handle == 0) {
		printf("At head of CleanEnableDevice():\n");
		printf("\tERROR: Invalid device handle passed for %s at node %ihh.\n",
				controller->Name, controller->NodeId);

		return 0x2000000B;
	}

	uint32_t error_code = 0;
	int ret;

	ret = VCS_ClearFault(device_handle, controller->NodeId, &error_code);
	if (ret == 0) {
		printf("While clearing fault from %s at %s:\n\t", controller->Name, controller->Port);
		PrintError(error_code);
		return error_code;
	}

	ret = VCS_SetEnableState(device_handle, controller->NodeId, &error_code);
	controller->State = CTRL_STATE_OPENED;
	return error_code;
}

uint32_t CloseDevice(struct Controller* controller, void* device_handle) {
	if (device_handle == 0) {
		printf("At head of CloseDevice():\n");
		printf("\tERROR: Invalid device handle passed for %s at node %ihh.\n",
				controller->Name, controller->NodeId);

		return 0x2000000B;
	}

	uint32_t error_code = 0;
	int ret;

	ret = VCS_CloseDevice(device_handle, &error_code);
	if (ret == 0) {
		printf("While attempting to close device %s at node %ihh\n\t",
				controller->Name, controller->NodeId);
		PrintError(error_code);
		return error_code;
	}

	ret = VCS_SetDisableState(device_handle, controller->NodeId, &error_code);
	controller->State = CTRL_STATE_CLOSED;
	return error_code;
}

uint32_t CloseDevices(struct Controller controllers[], void* device_handles[], uint8_t num) {
	if (num < 1) { printf("Invalid device array size.\n"); return 0x06040043; } // General Parameter Error
	if (device_handles[0] == 0) {
		printf("At head of CloseDevices():\n");
		printf("\tERROR: Invalid gateway device handle passed for %s at node %ihh.\n",
				controllers[0].Name, controllers[0].NodeId);

		return 0x2000000B;
	}


	uint32_t error_code = 0;
	int ret;

	for (uint8_t i = num - 1; i == 0; i--) {
		void* handle = device_handles[i];
		if (handle == 0) { continue; }

		ret = VCS_CloseSubDevice(device_handles[i], &error_code);
		if (ret == 0) {
			printf("While closing subdevice %s at node %ihh:\n\t",
					controllers[i].Name, controllers[i].NodeId);
			PrintError(error_code);

			return error_code;
		}

		ret = VCS_SetDisableState(device_handles[i], controllers[i].NodeId, &error_code);
		controllers[i].State = CTRL_STATE_CLOSED;
	}

	ret = VCS_CloseDevice(device_handles[0], &error_code);
	if (ret == 0) {
		printf("While closing gateway device %s at node %ihh:\n\t",
				controllers[0].Name, controllers[0].NodeId);
		PrintError(error_code);
		return error_code;
	}

	return error_code;
}

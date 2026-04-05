#include <stdio.h>
#include <string.h>
#include "epos2.h"

const char* DEFAULT_NAME = "EPOS2";

int DecodeError(const uint32_t error_code, char* error_msg, uint8_t max_size) {
	int ret = VCS_GetErrorInfo(error_code, error_msg, max_size);
	if (ret == 0) { printf("ERROR: Failed to decode error\n"); }

	return ret;
}

int PrintError(const uint32_t error_code) {
	char msg[64] = { 0 };
	int ret = DecodeError(error_code, msg, 64);
	printf("ERROR 0x%X: %s\n", error_code, msg);

	return ret;
}

uint32_t ResetDevice(void *device_handle, struct Controller *controller_in) {
	if (device_handle == 0) { return 0x2000000B; }
	uint32_t error_code = 0;
	int ret = VCS_ResetDevice(device_handle, controller_in->NodeId, &error_code);
	if (ret == 0) {
		PrintError(error_code);
		printf("Device failed to be reset: %s\n", controller_in->Name);
	}

	return error_code;
}

uint32_t SetupTestrigAsCANGateway(struct Controller controllers[3], void* handles[3], int default_init) {
	uint32_t error_code = 0;

	if (default_init != 0) {
		for (uint8_t i = 0; i < 3; i++) {
			struct Controller controller = controllers[i];
			strncpy(controller.Name, "EPOS4", 6); // Initial testing shows this
			strncpy(controller.Protocol, "CANopen", 8);
			strncpy(controller.Interface, "", 1); // TOOD: discover this when the cables are here
			strncpy(controller.Port, "CAN0", 5); // TODO: discover when cables arrive
		}
	}
	
	error_code = InitializeDevices(controllers, handles, 3);
	if (error_code != 0) {
		printf("When initializing multiple devices: ");
		PrintError(error_code);
		return error_code;
	}

	return error_code;
}

uint32_t CleanupTestrig(struct Controller controllers[3], void* handles[3]) {
	uint32_t error_code = 0;
	
	error_code = CloseDevices(controllers, handles, 3);
	if (error_code != 0) {
		printf("When cleaning up multiple devices: ");
		PrintError(error_code);
		return error_code;
	}

	return error_code;
}

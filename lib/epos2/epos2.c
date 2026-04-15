#include <stdio.h>
#include <string.h>
#include "epos2.h"

const char* DEFAULT_NAME = "EPOS2"; // NOLINT

int vscl_decode_error(const uint32_t error_code, char* error_msg, vscl_byte_t max_size) {
	int ret = VCS_GetErrorInfo(error_code, error_msg, max_size);
	if (ret == 0) { printf("ERROR: Failed to decode error\n"); }

	return ret;
}

int vscl_print_error(const uint32_t error_code) {
	char msg[64] = { 0 };
	int ret = vscl_decode_error(error_code, msg, 64);
	printf("ERROR 0x%X: %s\n", error_code, msg);

	return ret;
}

uint32_t vscl_reset_device(void *device_handle, struct controller* controller_in) {
	if (device_handle == 0) { return 0x2000000B; }
	uint32_t error_code = 0;
	int ret = VCS_ResetDevice(device_handle, controller_in->node_id, &error_code);
	if (ret == 0) {
		vscl_print_error(error_code);
		printf("Device failed to be reset: %s\n", controller_in->name);
	}

	return error_code;
}

uint32_t vscl_setup_testrig_as_can_gateway(struct controller controllers[3], void* handles[3], int default_init) {
	uint32_t error_code = 0;

	if (default_init != 0) {
		for (vscl_byte_t i = 0; i < 3; i++) {
			struct controller controller = controllers[i];
			strncpy(controller.name, "EPOS4", 6); // Initial testing shows this
			strncpy(controller.protocol, "CANopen", 8);
			strncpy(controller.conn_interface, "", 1); // TOOD: discover this when the cables are here
			strncpy(controller.port, "CAN0", 5); // TODO: discover when cables arrive }
		}
	}
	
	error_code = vscl_initialize_devices(controllers, handles, 3);
	if (error_code != 0) {
		printf("When initializing multiple devices: ");
		vscl_print_error(error_code);
		return error_code;
	}

	return error_code;
}

uint32_t vscl_cleanup_testrig(struct controller controllers[3], void* handles[3]) {
	uint32_t error_code = 0;
	
	error_code = vscl_close_devices(controllers, handles, 3);
	if (error_code != 0) {
		printf("When cleaning up multiple devices: ");
		vscl_print_error(error_code);
		return error_code;
	}

	return error_code;
}

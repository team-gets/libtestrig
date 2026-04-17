#include <stdio.h>
#include <assert.h>

#include "epos2.h"
#include "connect.h"
#include "definitions.h"
#include "identify.h"

static void vscl_failed_open_device(uint32_t error_code) {
	vscl_print_error(error_code);
	printf("Failed to open device with with following characteristics:\n");
}

uint32_t vscl_initialize_device(struct controller* controller_out, void* node, uint8_t node_id) {
	uint32_t error_code = 0;

	node = VCS_OpenDevice(controller_out->name,
			controller_out->protocol,
			controller_out->conn_interface,
			controller_out->port,
			&error_code);

	controller_out->node_id = node_id;

	if (node == 0 || error_code != 0) {
		vscl_failed_open_device(error_code);
		vscl_print_characteristics(controller_out);
		return error_code;
	}
	
	error_code = vscl_clean_enable_device(controller_out, node);
	return error_code;
}

uint32_t vscl_initialize_devices(struct controller controllers_out[], void* handles_out[], uint8_t num) {
	if (num < 1) { printf("Invalid device array size.\n"); return 0x06040043; } // "General Parameter Error"
	uint32_t error_code = 0;

	handles_out[0] = VCS_OpenDevice(controllers_out[0].name,
			controllers_out[0].protocol,
			controllers_out[0].conn_interface,
			controllers_out[0].port,
			&error_code);

	controllers_out[0].node_id = 1;

	if (handles_out[0] == 0 || error_code != 0) {
		vscl_failed_open_device(error_code);
		vscl_print_characteristics(&controllers_out[0]);
		return error_code;
	}

	error_code = vscl_clean_enable_device(&controllers_out[0], handles_out[0]);
	if (error_code != 0) {
		printf("While attempting to open gateway:\n\t");
		vscl_print_error(error_code);

		return error_code;
	}

	for (uint8_t i = 1; i < num; i++) {
		if (handles_out[i] == NULL) { break; }

		handles_out[i] = VCS_OpenSubDevice(handles_out[i],
				controllers_out[i].name,
				controllers_out[i].protocol,
				&error_code);

		controllers_out[i].node_id = i + 1;

		if (handles_out[i] == 0 || error_code != 0) {
			vscl_failed_open_device(error_code);
			vscl_print_characteristics(&controllers_out[i]);
			return error_code;
		}

		error_code = vscl_clean_enable_device(&controllers_out[i], handles_out[i]);
		if (error_code != 0) {
			printf("While attempting to open gateway:\n\t");
			vscl_print_error(error_code);
			return error_code;
		}
	}

	return 0;
} // uint32_t InitializeThreeDevices

uint32_t vscl_clean_enable_device(struct controller* controller, void* device_handle) {
	if (device_handle == 0) {
		printf("At head of CleanEnableDevice():\n");
		printf("\tERROR: Invalid device handle passed for %s at node %ihh.\n",
				controller->name, controller->node_id);

		return 0x2000000B;
	}

	uint32_t error_code = 0;
	int ret;

	ret = VCS_ClearFault(device_handle, controller->node_id, &error_code);
	if (ret == 0) {
		printf("While clearing fault from %s at %s:\n\t", controller->name, controller->port);
		vscl_print_error(error_code);
		return error_code;
	}

	ret = VCS_SetEnableState(device_handle, controller->node_id, &error_code);
	controller->state = CTRL_STATE_OPENED;
	return error_code;
}

uint32_t vscl_close_device(struct controller* controller, void* device_handle) {
	if (device_handle == 0) {
		printf("At head of CloseDevice():\n");
		printf("\tERROR: Invalid device handle passed for %s at node %ihh.\n",
				controller->name, controller->node_id);

		return 0x2000000B;
	}

	uint32_t error_code = 0;
	int ret;

	ret = VCS_CloseDevice(device_handle, &error_code);
	if (ret == 0) {
		printf("While attempting to close device %s at node %ihh\n\t",
				controller->name, controller->node_id);
		vscl_print_error(error_code);
		return error_code;
	}

	ret = VCS_SetDisableState(device_handle, controller->node_id, &error_code);
	controller->state = CTRL_STATE_CLOSED;
	return error_code;
}

uint32_t vscl_close_devices(struct controller controllers[], void* device_handles[], uint8_t num) {
	if (num < 1) { printf("Invalid device array size.\n"); return 0x06040043; } // General Parameter Error
	if (device_handles[0] == 0) {
		printf("At head of CloseDevices():\n");
		printf("\tERROR: Invalid gateway device handle passed for %s at node %ihh.\n",
				controllers[0].name, controllers[0].node_id);

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
					controllers[i].name, controllers[i].node_id);
			vscl_print_error(error_code);

			return error_code;
		}

		ret = VCS_SetDisableState(device_handles[i], controllers[i].node_id, &error_code);
		controllers[i].state = CTRL_STATE_CLOSED;
	}

	ret = VCS_CloseDevice(device_handles[0], &error_code);
	if (ret == 0) {
		printf("While closing gateway device %s at node %ihh:\n\t",
				controllers[0].name, controllers[0].node_id);
		vscl_print_error(error_code);
		return error_code;
	}

	return error_code;
}

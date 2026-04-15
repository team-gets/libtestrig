#include <stdio.h>
#include "manage.h"

uint32_t rig_abort(const struct controller* controller_in, void* device_handle) {
	uint32_t error_code = 0;
	if (device_handle == 0) {
		printf("At head of RigAbort():\n");
		printf("\tERROR: Invalid device handle passed for %s at node %ihh.\n",
				controller_in->name, controller_in->node_id);
		return 0x2000000B;
	}

	printf("WARNING: Attempting to call abort on %s of node %i!!\n",
			controller_in->name, controller_in->node_id);

	int ret = VCS_SetQuickStopState(device_handle, controller_in->node_id, &error_code);
	if (ret == 0) {
		print_error(error_code);
		printf("DANGER: Failed to abort rig operations!\n");
	}
	else {
		printf("WARNING: Successfully aborted operations.\n");
	}

	return error_code;
}

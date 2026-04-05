#include <stdio.h>
#include "manage.h"

uint32_t RigAbort(const struct Controller* controller_in, void* device_handle) {
	uint32_t error_code = 0;
	if (device_handle == 0) {
		printf("At head of RigAbort():\n");
		printf("\tERROR: Invalid device handle passed for %s at node %ihh.\n",
				controller_in->Name, controller_in->NodeId);
		return 0x2000000B;
	}

	printf("WARNING: Attempting to call abort on %s of node %i!!\n",
			controller_in->Name, controller_in->NodeId);

	int ret = VCS_SetQuickStopState(device_handle, controller_in->NodeId, &error_code);
	if (ret == 0) {
		PrintError(error_code);
		printf("DANGER: Failed to abort rig operations!\n");
	}
	else {
		printf("WARNING: Successfully aborted operations.\n");
	}

	return error_code;
}

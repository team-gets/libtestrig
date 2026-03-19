#include <stdio.h>
#include <string.h>

#include "controller.h"
#include "definitions.h"
#include "identify.h"

unsigned int AcquireDeviceInformation(struct Controller* controller_out, int flags) {
	unsigned int error_code = 0;
	int success;
	int selection_end;
	
	if (flags & (1 << 2)) { memset(controller_out, 0, sizeof(*controller_out)); }

	if (flags & (1 << 1)) {
		strncpy(controller_out->Name, "EPOS2", 6);
		strncpy(controller_out->Protocol, "CANopen", 8);
	}

	success = VCS_GetInterfaceNameSelection(
			controller_out->Name, controller_out->Protocol, 1,
			controller_out->Interface, 64, &selection_end, &error_code);

	return (success == 0 && error_code == 0) ? 0 : error_code;
}

void PrintControllerCharacteristics(struct Controller* controller) {
	printf("- Name:      %s\n", controller->Name);
	printf("- Node:      %i\n", controller->NodeId);
	printf("- Protocol:	 %s\n", controller->Protocol);
	printf("- Interface: %s\n", controller->Interface);
}

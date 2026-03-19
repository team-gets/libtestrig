#include <stdio.h>
#include <string.h>

#include "controller.h"
#include "definitions.h"
#include "identify.h"

unsigned int AcquireFirstDeviceInformation(struct Controller* controller_out, int flags) {
	unsigned int error_code = 0;
	int selection_end;
	
	if (flags & (1 << 2)) { memset(controller_out, 0, sizeof(*controller_out)); }

	if (flags & (1 << 1)) {
		strncpy(controller_out->Name, "EPOS2", 6);
		strncpy(controller_out->Protocol, "CANopen", 8);
	}
	else {
		int name_found = VCS_GetDeviceNameSelection(
				1, controller_out->Name, 8,
				&selection_end, &error_code);
		if (name_found == 0) { return error_code; }

		int protocol_found = VCS_GetProtocolStackNameSelection(
				controller_out->Name, 1,
				controller_out->Protocol, 16,
				&selection_end, &error_code);
		if (protocol_found == 0) { return error_code; }
	}

	int interface_found = VCS_GetInterfaceNameSelection(
			controller_out->Name, controller_out->Protocol, 1,
			controller_out->Interface, 64, &selection_end, &error_code);
	if (interface_found == 0) { return error_code; }

	int port_found = VCS_GetPortNameSelection(
			controller_out->Name, controller_out->Protocol, controller_out->Interface, 1,
			controller_out->Port, 8, &selection_end, &error_code);
	if (port_found == 0) { return error_code; }

	return 0;
}

void PrintControllerCharacteristics(struct Controller* controller) {
	printf("- Name:      %s\n", controller->Name);
	printf("- Node:      %i\n", controller->NodeId);
	printf("- Protocol:	 %s\n", controller->Protocol);
	printf("- Interface: %s\n", controller->Interface);
}

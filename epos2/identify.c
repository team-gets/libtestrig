#include <stdio.h>
#include <string.h>

#include "controller.h"
#include "definitions.h"
#include "identify.h"

unsigned int AcquireDeviceNames(struct Controller** controllers_out, int size) {
	int n = 0;
	unsigned int error_code = 0;
	int selection_end;

	int name_found = VCS_GetDeviceNameSelection(
			1, controllers_out[0]->Name, 8,
			&selection_end, &error_code);
	if (name_found != 0) { return error_code; }
	n++;

	while (!selection_end && n < size) {
		name_found = VCS_GetDeviceNameSelection(
				0, controllers_out[n]->Name, 8,
				&selection_end, &error_code);

		if (name_found != 0) { return error_code; }
		n++;
	}

	return error_code;
}

unsigned int AcquireDeviceProtocols(struct Controller** controllers_out, int size) {
	int n = 0;
	unsigned int error_code = 0;
	int selection_end;

	int protocol_found = VCS_GetProtocolStackNameSelection(
			controllers_out[0]->Name, 1,
			controllers_out[0]->Protocol, 16,
			&selection_end, &error_code);
	if (protocol_found == 0) { return error_code; }
	n++;

	while (!selection_end && n < size) {
		protocol_found = VCS_GetProtocolStackNameSelection(
				controllers_out[n]->Name, 0,
				controllers_out[n]->Protocol, 16,
				&selection_end, &error_code);

		if (protocol_found == 0) { return error_code; }
		n++;
	}

	return error_code;
}

unsigned int AcquireDeviceInterfaces(struct Controller** controllers_out, int size) {
	int n = 0;
	unsigned int error_code = 0;
	int selection_end;

	int interface_found = VCS_GetInterfaceNameSelection(
			controllers_out[0]->Name, controllers_out[0]->Protocol, 1,
			controllers_out[0]->Interface, 64, &selection_end, &error_code);
	if (interface_found == 0) { return error_code; }
	n++;

	while (!selection_end && n < size) {
		interface_found = VCS_GetInterfaceNameSelection(
				controllers_out[n]->Name, controllers_out[n]->Protocol, 0,
				controllers_out[n]->Interface, 64, &selection_end, &error_code);

		if (interface_found == 0) { return error_code; }
		n++;
	}

	return error_code;
}

unsigned int AcquireDevicePorts(struct Controller** controllers_out, int size) {
	int n = 0;
	unsigned int error_code = 0;
	int selection_end;

	int port_found = VCS_GetPortNameSelection(
			controllers_out[0]->Name, controllers_out[0]->Protocol, controllers_out[0]->Interface, 1,
			controllers_out[0]->Port, 8, &selection_end, &error_code);
	if (port_found == 0) { return error_code; }
	n++;

	while (!selection_end && n < size) {
		port_found = VCS_GetPortNameSelection(
				controllers_out[n]->Name, controllers_out[n]->Protocol, controllers_out[n]->Interface, 0,
				controllers_out[n]->Port, 8, &selection_end, &error_code);
		if (port_found == 0) { return error_code; }
		n++;
	}

	return error_code;
}

unsigned int AcquireDeviceInfo(struct Controller** controllers_out, int size, int flags) {
	unsigned int error_code = 0;
	
	if (flags & (1 << 2)) { memset(controllers_out, 0, sizeof(*controllers_out)); }

	if (flags & (1 << 1)) {
		for (int i = 0; i < size; i++) {
			strncpy(controllers_out[i]->Name, "EPOS2", 6);
			strncpy(controllers_out[i]->Protocol, "CANopen", 8);
		}
	}
	else {
		error_code = AcquireDeviceNames(controllers_out, size);
		if (error_code != 0) { return error_code; };

		error_code = AcquireDeviceProtocols(controllers_out, size);
		if (error_code != 0) { return error_code; };
	}

	error_code = AcquireDeviceInterfaces(controllers_out, size);
	if (error_code != 0) { return error_code; }

	error_code = AcquireDevicePorts(controllers_out, size);
	if (error_code != 0) { return error_code; }

	return 0;
}

void PrintControllerCharacteristics(struct Controller* controller) {
	printf("- Name:      %s\n", controller->Name);
	printf("- Node:      %i\n", controller->NodeId);
	printf("- Protocol:	 %s\n", controller->Protocol);
	printf("- Interface: %s\n", controller->Interface);
}

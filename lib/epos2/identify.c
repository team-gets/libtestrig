#include <stdio.h>

#include "epos2.h"
#include "controller.h"
#include "definitions.h"
#include "identify.h"

static void PrintNamingError(uint8_t error_code) {
	printf("While identifying device names:\n\t");
	PrintError(error_code);
}

static void PrintProtocolIdentError(uint8_t error_code, const char* name) {
	printf("While identifying device protocol stacks for device %s:\n\t", name);
	PrintError(error_code);
}

static void PrintInterfaceIdentError(uint8_t error_code, const char* name, const char* protocol) {
	printf("While identifying device interface for device %s on protocol %s:\n\t", name, protocol);
	PrintError(error_code);
}

static void PrintPortIdentError(uint8_t error_code, const char* name, const char* protocol, const char* iface) {
	printf("While identifying device interface for device %s on protocol %s and interface %s:\n\t",
			name, protocol, iface);
	PrintError(error_code);
}

uint32_t IdentifyDeviceNames(void) {
	uint32_t error_code = 0;
	int n = 1;
	int selection_end;
	char name[8] = { 0 };

	int name_found = VCS_GetDeviceNameSelection(
			1, name, 8,
			&selection_end, &error_code);
	if (name_found != 0) {
		PrintNamingError(error_code);
		return error_code;
	}

	printf("Found device name(s):\n");
	printf("%i. %s\n", n, name);
	n++;

	while (selection_end == 0) {
		char subname[8] = { 0 };
		name_found = VCS_GetDeviceNameSelection(
			0, subname, 8,
			&selection_end, &error_code);

		if (name_found != 0) {
			PrintNamingError(error_code);
			return error_code;
		}

		printf("%i. %s\n", n, name);
		n++;
	}

	return error_code;
}

uint32_t IdentifyDeviceProtocols(char* device_name) {
	uint32_t error_code = 0;
	int n = 1;
	int selection_end;
	char protocol[16] = { 0 };

	int protocol_found = VCS_GetProtocolStackNameSelection(
			device_name, 1,
			protocol, 16,
			&selection_end, &error_code);

	if (protocol_found == 0) {
		PrintProtocolIdentError(error_code, device_name);
		return error_code;
	}

	printf("Found device protocol stacks associated with %s(s):\n", device_name);
	printf("%i. %s\n", n, protocol);
	n++;

	while (selection_end == 0) {
		char subprotocol[16] = { 0 };
		protocol_found = VCS_GetProtocolStackNameSelection(
			device_name, 0,
			subprotocol, 16,
			&selection_end, &error_code);

		if (protocol_found == 0) {
			PrintProtocolIdentError(error_code, device_name);
			return error_code;
		}

		printf("%i. %s\n", n, protocol);
		n++;
	}

	return error_code;
}

uint32_t IdentifyDeviceInterfaces(char* device_name, char* protocol_stack) {
	uint32_t error_code = 0;
	int n = 1;
	int selection_end;
	char device_interface[64] = { 0 };

	int interface_found = VCS_GetInterfaceNameSelection(
			device_name, protocol_stack, 1,
			device_interface, 64, &selection_end, &error_code);

	if (interface_found == 0) {
		PrintInterfaceIdentError(error_code, device_name, protocol_stack);
		return error_code;
	}

	printf("Found device protocol stacks associated with %s and %s (s):\n", device_name, protocol_stack);
	printf("%i. %s\n", n, device_interface);
	n++;

	while (selection_end == 0) {
		char subdevice_interface[64] = { 0 };
		interface_found = VCS_GetInterfaceNameSelection(
				device_name, protocol_stack, 0,
				subdevice_interface, 64, &selection_end, &error_code);

		if (interface_found == 0) {
			PrintInterfaceIdentError(error_code, device_name, protocol_stack);
			return error_code;
		}

		printf("%i. %s\n", n, device_interface);
		n++;
	}

	return error_code;
}

uint32_t IdentifyDevicePorts(char* device_name, char* protocol_stack, char* device_interface) {
	uint32_t error_code = 0;
	int n = 1;
	int selection_end;
	char device_port[8] = { 0 };

	int port_found = VCS_GetPortNameSelection(
			device_name, protocol_stack, device_interface, 1,
			device_port, 8, &selection_end, &error_code);
	if (port_found == 0) {
		PrintPortIdentError(error_code, device_name, protocol_stack, device_interface);
		return error_code;
	}

	printf("Found device protocol stacks associated with %s, %s, and %s (s):\n",
			device_name, protocol_stack, device_interface);
	printf("%i. %s\n", n, device_port);
	n++;

	while (selection_end == 0) {
		char subdevice_port[8] = { 0 };
		port_found = VCS_GetPortNameSelection(
				device_name, protocol_stack, device_interface, 0,
				subdevice_port, 8, &selection_end, &error_code);

		if (port_found == 0) {
			PrintPortIdentError(error_code, device_name, protocol_stack, device_interface);
			return error_code;
		}

		printf("%i. %s\n", n, device_port);
		n++;
	}

	return error_code;
}


void PrintControllerCharacteristics(struct Controller* controller) {
	printf("- Name:          %s\n", controller->Name);
	printf("- Node:          %i\n", controller->NodeId);
	printf("- Protocol:	     %s\n", controller->Protocol);
	printf("- Interface:     %s\n", controller->Interface);
	printf("- Port:          %s\n", controller->Port);
}

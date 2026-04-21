#include <string.h>
#include "message.h"

// All of these are magic numbers btw
const vscl_byte_t HEAD_STAY[4] = { 27, 34, 41, 48 };
const vscl_byte_t HEAD_DC[4] = { 255, 127, 63, 31 };
const vscl_byte_t HEAD_SYNC[4] = { 'S', 'Y', 'N', 'C' };
const vscl_byte_t HEAD_IDENT[4] = { 'I', 'D', 234, 137 };
const vscl_byte_t MESSAGE_BLANK[8] = { 0 };

int vscl_set_message(struct rig_message* msg, const vscl_byte_t* head, const vscl_byte_t* data) {
    memset(msg, 0, sizeof(struct rig_message));
	memcpy(msg->head, head, 4);
	memcpy(msg->data, data, 8);
	return 0;
}

int vscl_ident_header_part(vscl_byte_t in[4], int idx) {
	if (in[idx] == HEAD_STAY[idx]) {
		return HEADER_IS_STAY;
	}
	else if (in[idx] == HEAD_DC[idx]) {
		return HEADER_IS_DC;
	}
	else if (in[idx] == HEAD_SYNC[idx]) {
		return HEADER_IS_SYNC;
	}
	else {
		return -1;
	}
}

int vscl_ident_full_header(vscl_byte_t in[4]) {
	int identity = -1;

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			identity = vscl_ident_header_part(in, 0);
		}
		else {
			identity &= vscl_ident_header_part(in, i);
		}
	}

	return identity;
}

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

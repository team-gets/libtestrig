#include <string.h>

#include "message.h"

int SetMessage(struct RigMessage* msg, const uint8_t* head, const uint8_t* data) {
	memcpy(msg->head, head, 4);
	memcpy(msg->data, data, 8);
	return 0;
}

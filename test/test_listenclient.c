#include <stdio.h>
#include <stdint.h>

#include "ipc/ipc.h"
#include "ipc/message.h"
#include "ipc/constants.h"

int Handle(uint8_t* buf) {
	int header = IdentifyFullHeader(buf);

	switch (header) {

	}

	return HANDLER_RET_DC;
}

int main(int argc, char** argv) {
	int retstat;
	int sock;
	struct sockaddr_un sockaddr;

	sock = SockSetup(&sockaddr);
	if (sock == -1) { perror("listen setup failure"); return -1; }

	retstat = SockBind(sock, &sockaddr);
	if (retstat == -1) { perror("listen binding failure"); return -1; }

	retstat = SockReadAndHandle(sock, &sockaddr, &Handle);
	if (retstat == -1) { perror("listen reading failure"); return -1; }
	
	return 0;
}

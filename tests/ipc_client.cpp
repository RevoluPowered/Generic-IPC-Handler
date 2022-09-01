#include "src/ipc.h"

#define SOCKET_NAME "ipc_test.socket"

void Receive(const char *string, int strlen) {
	printf("client got message: %s length %d\n", string, strlen);
}

int main() {
	IPCClient client;
	client.add_receive_callback(&Receive);
	char hello[] = { "Hello World!\0" };
	if (client.setup_one_shot(SOCKET_NAME, hello, strlen(hello))) {
		client.poll_update();
	}
	return 0;
}

#include "src/ipc.h"

#define SOCKET_NAME "ipc_test.socket"

void Receive(const char *string, int strlen) {
	printf("server got message: %s\n", string);
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

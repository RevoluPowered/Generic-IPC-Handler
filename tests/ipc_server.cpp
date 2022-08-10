#include "src/ipc.h"

#define SOCKET_NAME "ipc_test.socket"

void Receive(const char *string, size_t strlen) {
	printf("server got message: %s length %d\n", string, int(strlen));
}

int main() {
	IPCServer server;
	printf("Starting server\n");
	server.add_receive_callback(&Receive);
	if (server.setup(SOCKET_NAME)) {
		printf("Server setup completed, starting polling\n");
		while (server.poll_update()) {
			// do nothing
		};
	}
	return 0;
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "src/ipc.h"
#include "thirdparty/doctest.h"

#define SOCKET_NAME "ipc_test.socket"

#include <thread>
using namespace std;

atomic_bool keep_server_on, server_updated;
void ServerThread() {
	IPCServer server;
	// ensure server starts
	CHECK(server.setup(SOCKET_NAME));

	bool poll_update;
	while (keep_server_on) {
		poll_update = server.poll_update();
		CHECK(poll_update);
		server_updated = true;
		//printf("server update\n");
	}
}

TEST_CASE("hello test") {
	IPCClient client;
	keep_server_on = true;
	server_updated = false;
	// ensure failure
	char hello[] = { "Hello World!\0" };

	thread server(ServerThread);
	bool first = true;
	while (!server_updated) {
		if (first) {
			//printf("waiting for server\n");
			first = false;
		}
	}
	CHECK(client.setup_one_shot(SOCKET_NAME, hello, strlen(hello)));
	keep_server_on = false;
	server.join(); // wait for close
}

TEST_CASE("varying delays sending test") {
	IPCClient client;
	keep_server_on = true;
	server_updated = false;
	// ensure failure

	//CHECK(client.setup_one_shot(SOCKET_NAME, hello, strlen(hello) ) == false);

	thread server(ServerThread);
	bool first = true;
	while (!server_updated) {
		if (first) {
			printf("waiting for server\n");
			first = false;
		}
	}

	for (int x = 0; x < 100000; x++) {
		char hello[256];
		int n = sprintf(hello, "hello %d", x);
		CHECK_GT(n, 0);
		//char hello[] = "Hello World!" + x;
		CHECK(client.setup_one_shot(SOCKET_NAME, hello, strlen(hello)));
	}
	keep_server_on = false;
	server.join(); // wait for close
}

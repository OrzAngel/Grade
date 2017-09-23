// server_shut_down_tester.c

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "server_shut_down.h"

int main() {

	shut_down_thread_init();

	while (!is_shut_down())
	{
		fprintf(stdout, "Server is running\n");
		sleep(2);
	}

	shut_down_thread_join();

	return 0;

}

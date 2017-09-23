CC = clang
ARGS = -lpthread


all: server

server: httpserver.c deal_request.c database.c server_shut_down.c thread_list.c
	$(CC) -o server $(ARGS) httpserver.c deal_request.c database.c server_shut_down.c thread_list.c

shut_down_test: server_shut_down.c server_shut_down_tester.c
	$(CC) -o shut_down_test server_shut_down.c server_shut_down_tester.c $(ARGS)	

database_test: database_tester.c database.c
	$(CC) -o database_test database_tester.c database.c  	

clean: 
	rm -rf server server shut_down_test database_test *.o

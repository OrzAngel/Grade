/*
 * This file is the entry point of the whole project.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "deal_request.h"
#include "server_shut_down.h"
#include "database.h"
#include "thread_list.h"

/*
 * Start a socket server binding to and listening on the input port number
 * This code primarily comes from the code httpserver.c on canvas Chris provided for us.
 * return the socket descriptor
 */
int start_server(int PORT_NUMBER)
{
    
    // structs to represent the server and client
    struct sockaddr_in server_addr;
    
    int sock; // socket descriptor
    
    // 1. socket: creates a socket descriptor that you later use to make other system calls
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        //socket creates the socket descriptor, information about the socket struct
        perror("Socket");
        exit(1);
    }
    int temp = 0;
    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
    }
    
    // configure the server
    server_addr.sin_port = htons(PORT_NUMBER); // specify port number
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero),8);
    
    // 2. bind: use the socket and associate it with the port number
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }
    
    // 3. listen: indicates that we want to listen to the port to which we bound; second arg is number of allowed connections
    if (listen(sock, 1) == -1) {
        perror("Listen");
        exit(1);
    }
    
    char* server_ip = inet_ntoa(server_addr.sin_addr);
    // once you get here, the server is set up and about to start listening
    printf("\nServer configured to listen on %s : %d\n",server_ip, PORT_NUMBER);
    fflush(stdout);
    dr_set_base(PORT_NUMBER,server_ip);

    return sock;
    
}

/*
 * The entry point of the project
 * If the filename of the datas is not included in the command line argument, a default one is used.
 */
int main(int argc, char *argv[])
{
    
    // check the number of arguments

    if (argc != 2 && argc != 3) {
        printf("\nUsage: %s [port_number] or %s [port_number] [file_name]\n", argv[0], argv[0]);
        exit(-1);
    }

    char* filename;
    if(argc == 3){
        filename = argv[2];
    }
    else{
        filename = "course_evals.txt";
    }

    int port_number = atoi(argv[1]);
    if (port_number <= 1024) {
        printf("\nPlease specify a port number greater than 1024\n");
        exit(-1);
    }
    
    
    int r;
    int sock;
    sock = start_server(port_number);
    
    int i = 1;

    shut_down_thread_init();
    database_init(700, filename);
    tl_init();

    while(!is_shut_down()){
        
        struct sockaddr_in client_addr;
        
        // 4. accept: wait here until we get a connection on that port
        int sin_size = sizeof(struct sockaddr_in);
        int *fd = malloc(sizeof(int));
        *fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
        if (*fd != -1) {
            printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            
            
            pthread_t* threads = tl_get();
            if(threads == NULL)
            {
                return -1;
            }

            r = pthread_create(threads, NULL, deal_request, fd);
            if(r !=0){
                perror("pthread_create");
                exit(1);
            }

        }
        i++;
    }
    
    shut_down_thread_join();
    tl_join();
    tl_clear();
    database_free();
    // 8. close: close the socket
    close(sock);
    printf("Server shutting down, %d requests have been responded\n",i);
    
    return 0;
}
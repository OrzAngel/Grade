#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "database.h"
#include "deal_request.h"

char table_header[2048];
char html_header[1024];
char base[1024];

char* get_reply(char* request);
void table_header_init();


/*
Initialize the base link of http
*/
void dr_set_base(int port_number, char* server_ip)
{
    sprintf(base,"<base href=\"http://%s:%d/\">",server_ip,port_number);

    dr_set_port_number(0);

    table_header_init();

}

/*
Initialize the HTML table header that will display all the database by reading a set up file called table_header.html
*/
void table_header_init(){
    table_header[0] = '\0';
    FILE* src_file;
    char str1[100];
    src_file = fopen("table_header.html", "r");
        

    while(fgets(str1,99,src_file))
    {
        strcat(table_header,str1);
    }
        

    fclose(src_file);
       

}

/*
Cast the HTTP reply with proper headers
*/
void dr_set_port_number(int port_number)
{
    char* http_header = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
    sprintf(html_header,"%s<html>%s<a href=\"/index.html/\" target=\"_self\">Display</a>",http_header,base);
    strcat(html_header,"<form action = \"/sleep.html/\"> <input size = \"15\" type=\"text\" name=\"bar\"/> <input type=\"submit\" value = \"Sleep\"></form>");
    strcat(html_header,"<a href=\"/calculate/average/\" target=\"_self\">Average</a><br>");
    strcat(html_header,"<a href=\"/calculate/min/\" target=\"_self\">Min</a>");   
}

/*
Parse the coming HTTP request
Get the HTTP reply
Send the reply back
*/
void* deal_request(void* p){
    
    int fd = *(int*)p;
    free(p);
    char request[1024];

    
    // 5. recv: read incoming message (request) into buffer
    ssize_t bytes_received = recv(fd,request,1024,0);
    // null-terminate the string
    request[bytes_received] = '\0';
    
    // print it to standard out
    printf("This is the incoming request:\n%s\n", request);

    char* reply;
    char token[5];
    
    sscanf(request,"%s",token);
    // printf("token: %s\n", token);
    // printf("%s\n", request);
    if(strcmp(token,"GET") == 0){
        // this is the message that we'll send back
        reply = get_reply(request);
    } else {
        reply = "";
    }

    // 6. send: send the outgoing message (response) over the socket
    // note that the second argument is a char*, and the third is the number of chars
    send(fd, reply, strlen(reply), 0);

    free(reply);
    
    // 7. close: close the connection
    close(fd);
    printf("Server closed connection\n");
    
    return NULL;
}

/*
Parse the GET HTTP request
Generate the reply according to specific request
*/
char* get_reply(char* request){


    //printf("%s\n", request);
    
    char* reply = malloc(100000);
    if(reply == NULL){
        return NULL;
    }

    strcpy(reply,html_header);
    
    strcat(reply, "<table align=\"center\">");     
    strcat(reply, table_header);


    eval** e = NULL;

    char token[50];

    sscanf(request,"GET /%[^/]/",token);
    // printf("token: %s\n", token);

    if (strcmp(token,"index.html") == 0)
    {
        e = get_all();
    }

    if (strcmp(token,"sleep.html") == 0){
        int sleep_time;
        sscanf(request, "GET /sleep.html/?bar=%d",&sleep_time);
        sleep(sleep_time);
        e = get_all();
    }

    if (strcmp(token,"calculate") == 0){
        char column[20];
        sscanf(request, "GET /calculate/%[^/]/",column);
    
        e = get_all();
        
        if(strcmp(column, "average")==0){
            eval* avg = calculate_average(e);
            char* eval_average = eval_to_string(avg);
            // printf("%s\n", eval_average);
            strcat(reply, eval_average);
            free(avg-> course_number);
            free(avg-> instructor);
            free(avg);
            free(eval_average);
        }
        if(strcmp(column, "min")==0){
            eval* min = calculate_min(e);
            char* eval_min = eval_to_string(min);
            strcat(reply, eval_min);
            free(min-> course_number);
            free(min-> instructor);
            free(min);
            free(eval_min);
        }
        // e[0] = NULL;
    }

    if (strcmp(token,"sort") == 0)
    {   
        char column[20];
        char sorting[20];
        sscanf(request, "GET /sort/%[^/]/%[^/]/",column, sorting);
        e = get_sorted(column, sorting);
    }

    if (strcmp(token,"filter") == 0)
    {   
        char column[20];
        char bar_string[50];
        sscanf(request, "GET /filter/%[^/]/?bar=%s",column,bar_string);
        e = get_filtered(column, bar_string);
    }
    if (!e)
    {
        e = get_404();
    }

    int i = 0;


    while(e[i] != NULL){

        char* eval_info = eval_to_string(e[i]);
        strcat(reply, eval_info);
        
        free(eval_info);
        
        i++; 
    }

    strcat(reply, "</table></html>\n");

    free(e);

    return reply;

}





// server_shut_down.c
/*
 * This file implenents all the functions necessary to tell the outside world when to shut a server down.
 * When the shut_down_thread_init() function is called a new thread waiting for the user input is created.
 * Once the user type in the lowercase letter 'q', the state indicator SERVER_SHUT_DOWN would change.
 * By calling is_shut_down() function the outside user knows whether or not the server shouls shut down 
 * Mutex lock is applied to avoid race conditions.   	
 */
#include <stdio.h>
#include <termios.h>
#include <unistd.h>     //STDIN_FILENO
#include <pthread.h>
#include "server_shut_down.h"

int SERVER_SHUT_DOWN;
pthread_mutex_t sd_lock;
pthread_t       sd_thread;

/*
 * The entry point of a thread
 * Initialize the SERVER_SHUT_DOWN to 0.
 * Change SERVER_SHUT_DOWN to 1 if 'q' is pressed, meaning that it's about time to shut the server down
 * In order to avoid press the Enter key to get input the terminal configuration is changed.
 * The configuration code is modified based on the solution from 
 * http://stackoverflow.com/questions/1798511/how-to-avoid-press-enter-with-any-getchar
 * At least for now no unexpected behavior has been observed 
 */
void* shut_down(void* p) 
{	
	pthread_mutex_init(&sd_lock,NULL);
	pthread_mutex_lock(&sd_lock);
	SERVER_SHUT_DOWN = 0;	
	pthread_mutex_unlock(&sd_lock);
    static struct termios oldt, newt;

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);          

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

	while (getchar() !='q');

	pthread_mutex_lock(&sd_lock);
	SERVER_SHUT_DOWN = 1;	
	pthread_mutex_unlock(&sd_lock);

	fprintf(stdout, "\nServer start shut down\n");
	
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

	return NULL;
}

/*
 * create the shut_down thread
 */
int shut_down_thread_init()
{
	pthread_create(&sd_thread,NULL,&shut_down,NULL);
	return 0;
}

/*
 * join the shut_down thread
 */
int shut_down_thread_join()
{
	pthread_join(sd_thread,NULL);
	return 0;
}

/*
 * return 1 if the server should shut down at this time('q' has been pressed)
 * otherwise return 0
 */
int is_shut_down() {

	pthread_mutex_lock(&sd_lock);
	int res = SERVER_SHUT_DOWN;	
	pthread_mutex_unlock(&sd_lock);

	return res;
}

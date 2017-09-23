
typedef struct Node node;

struct Node {
  pthread_t* thread;    // pointer to a new thread
  node* next;   // pointer to next node in the list
};

pthread_t* tl_get(); 
void tl_init();
void tl_join();
void tl_clear();
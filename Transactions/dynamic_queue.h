// CMPUT 379 Louie Xie 1571535
// Assignment 3: dynamic_queue.h FROM Assignment 2
// Reference:   https://ece.uwaterloo.ca/~dwharder/aads/Projects/2/Dynamic_queue/
//              https://sites.ualberta.ca/~jhoover/ConcreteComputing/section/queues.htm

#ifndef INC_379A2_DYNAMIC_QUEUE_H
#define INC_379A2_DYNAMIC_QUEUE_H

#include <string.h>

// Struct for work
typedef struct client_node {
    char* index;
    char* host_name;
    struct client_node* next;
} client_node;

// Struct for dynamic queue
typedef struct client_queue {
    int size;
    client_node* front;
    client_node* rear;
} client_queue;

client_node* get_node(char* this_index, char* this_hostname) {
    client_node* the_node = malloc(sizeof(struct client_node));

    the_node->index = malloc(sizeof (char) * (strlen(this_index)));
    the_node->host_name = malloc(sizeof (char) * (strlen(this_hostname)));

    strcpy(the_node->index, this_index);
    strcpy(the_node->host_name, this_hostname);

    the_node->next = NULL;

    return the_node;
}

client_queue* get_queue() {
    client_queue* the_queue = malloc(sizeof(struct client_queue));
    the_queue->size = 0;
    the_queue->front = the_queue->rear = NULL;

}
void node_enqueue(client_queue* the_queue, char* index, char* host_name) {
    client_node * new_node = get_node(index, host_name);
    if (the_queue->front == NULL) {
        the_queue->front = new_node;
        the_queue->rear = new_node;
    } else {
        the_queue->rear->next = new_node;
        the_queue->rear = the_queue->rear->next;
    }
    the_queue->size += 1;
}

int get_size(client_queue* the_queue) {
    return the_queue->size;
}
bool is_empty(client_queue*  the_queue) {
    return (the_queue->front == NULL);
}

#endif //INC_379A2_DYNAMIC_QUEUE_H

/*
This code is reused from Fall 2017 CIS *2520 Assignment 2
*/
#include<stdio.h>
#include<stdlib.h>
#include"linkedListAPI.h"
#include"queue.h"

Queue* createQueue( void (*printFunction)(void *toBePrinted),

void (*deleteFunction)(void *toBeDeleted)){
    Queue *q = malloc(sizeof(Queue));
    q->list = initializeList(printFunction,deleteFunction);
    q->count = 0;
    return q;
}

void pop(Queue *q){
    if( q ){
        // Get the data at the top of the stack
        void* data = top(q);
        // Use the data to delete the node from the list
        int result = deleteNodeFromList(q->list, data );
        // If successful, we can lower our count
        if(result == EXIT_SUCCESS){
        q->count--;
        }
    }
}

void* top(Queue* q){
    return getFromFront(q->list);
}

int count(Queue* q){
    return q->count;
}

void push(Queue* q, void * data){
    insertSorted(q->list, data);
    q->count++;
}

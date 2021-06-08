/*
This code is reused from Fall 2017 CIS *2520 Assignment 2
*/
#include<stdio.h>
#include<stdlib.h>
#include"linkedListAPI.h"

typedef struct queue{
    List *list;
    int count;
}Queue;

Queue* createQueue( void (*printFunction)(void *toBePrinted),

void (*deleteFunction)(void *toBeDeleted));

void pop(Queue *q);

void* top(Queue* q);

int count(Queue* q);

void push(Queue* q, void * data);

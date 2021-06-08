/*
This code is reused from Fall 2017 CIS *2520 Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include "linkedListAPI.h"
List *initializeList(void (*printFunction)(void *toBePrinted),void (*deleteFunction)(void *toBeDeleted)){
    List *list = malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->printNode = printFunction;
    list->deleteNode = deleteFunction;
    return list;
}

Node *initializeNode(void *data){
    Node* current = malloc(sizeof(Node));
    current->next = NULL;
    current->data = data;
    current->previous = NULL;
    return current;
}

void insertFront(List *list, void *toBeAdded){
    Node *newElem = initializeNode(toBeAdded); 
    if(list->head == NULL && list->tail == NULL){
        list->head = newElem;
        list->tail = newElem;
    }
    else{
        newElem->next = list->head;
        list->head->previous = newElem;
        list->head = newElem;
    }     
}

void insertBack(List *list, void *toBeAdded){
    Node *newElem = initializeNode(toBeAdded);
    if(list->head == NULL && list->tail == NULL){
        list->head = newElem;
        list->tail = newElem;
    }
    else{
        newElem->previous = list->tail;
        list->tail->next = newElem;
        list->tail = newElem;
    }   
}
void deleteList(List *list){
    Node *cur = list->head;
    while(cur != NULL){
        list->deleteNode(cur->data);
        list->head = cur->next;
        free(cur);
        cur = list->head;
    }
    free(list);
}
void insertSorted(List *list, void *toBeAdded){
    Node *posn = list->head;
    Node *newElm =initializeNode(toBeAdded);
    if(list->head == NULL){
        list->head = newElm;
        list->tail = newElm;
    }
    else{
        Node *temp = NULL; 
        while(posn != NULL){
            temp = posn;
            posn = posn->next;
        }        
        if(temp == NULL){
            newElm->next = list->head;
            list->head->previous = newElm;
            list->head = newElm;
        }
        else if(posn == NULL){
            newElm->previous = list->tail;
            list->tail->next = newElm;
            list->tail = newElm; 
        }
        else{
            temp->next = newElm;
            newElm->previous = temp;
            newElm->next = posn;
            posn->previous = newElm; 
        }
    }
    
}
int deleteNodeFromList(List *list, void *toBeDeleted){
    if(list->head == NULL || toBeDeleted == NULL){
        return EXIT_FAILURE;
    }

    Node *cur = list->head;
    
    while(cur != NULL){
            list->deleteNode(cur->data);
            
            if(cur->previous == NULL && cur->next == NULL){
                list->head = NULL;
                list->tail = NULL;
            }else if(cur->previous == NULL){
                cur->next->previous = NULL;
                list->head = cur->next;
            }else if(cur->next == NULL){
                cur->previous->next = NULL;
                list->tail = cur->previous;
            }else{
                cur->next->previous = cur->previous;
                cur->previous->next = cur->next;
            }
            
            free(cur);
            return EXIT_SUCCESS;
        cur = cur->next;
    }

    return -1;

    
}
void *getFromFront(List *list){
    if(list->head == NULL){
        return NULL;
    }
    return list->head->data;
}
void *getFromBack(List *list){
    if(list->tail == NULL){
        return NULL;
    }
    return list->tail->data;
}
void printForward(List *list){
    Node *ptr = list->head;
    while(ptr != NULL){
        list->printNode(ptr->data);
        ptr = ptr->next;
    }
}
void printBackwards(List *list){
    Node *ptr = list->tail;
    while(ptr != NULL){
        list->printNode(ptr->data);
        ptr = ptr->previous;
    }
}

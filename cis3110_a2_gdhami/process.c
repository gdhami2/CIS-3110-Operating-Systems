#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "process.h"

Process *createProcess(int startTime, int id, int runTime, int exchangeTime[], int numExchange, int totRunTime, int exchangeCount, int timeinR, int totReadyTime,int timeinB, int totBlockedTime){
    Process *p = malloc(sizeof(Process));
    p->startTime = startTime;
    p->id = id;
    p->runTime = runTime;
    p->numExchange = numExchange;
    for(int i = 0; i <= numExchange; i++){
	p->exchangeTime[i] = exchangeTime[i];
    }
    p->totRunTime = totRunTime;
    p->exchangeCounter = exchangeCount;
    p->timeinR = timeinR;
    p->totReadyTime = totReadyTime;
    p->timeinB = timeinB;
    p->totBlockedTime = totBlockedTime;
    return p; 
}

int startTime(Process *p){
    return p->startTime;
}

int id(Process *p){
    return p->id;
}

int runTime(Process *p){
    return p->runTime;
}
int exchangeTime(Process *p, int i){
    return p->exchangeTime[i];
}
int numExchange(Process *p){
    return p->numExchange;
}
int totRunTime(Process *p){
    return p->totRunTime;
}
int exchangeCounter(Process *p){
    return p->exchangeCounter;
}
int timeinR(Process *p){
    return p->timeinR;
}
int totReadyTime(Process *p){
    return p->totReadyTime;
}
int timeinB(Process *p){
    return p->timeinB;
}
int totBlockedTime(Process *p){
    return p->totBlockedTime;
}

void deleteProcess(void *tobeDeleted){
    if(tobeDeleted != NULL){
        free(tobeDeleted);
    }
    tobeDeleted = NULL;
}

void printProcess(void * p) 
{
    Process *newProcess = (Process *) p;
    printf("%d %d %d\n", newProcess->id, newProcess->startTime, newProcess->runTime);
}

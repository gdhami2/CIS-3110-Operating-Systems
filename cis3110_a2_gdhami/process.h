#include <stdio.h>
#include <stdlib.h>

typedef struct process {
    int startTime;
    int id;
    int runTime;
    int timeinR;
    int timeinB;
    int exchangeTime[50];
    int numExchange;
    int exchangeCounter;
    int totRunTime;
    int totReadyTime;
    int totBlockedTime;
    int dummy;
} Process;

/**Function creates a process.
*@return the dynamically allocated process.
*@param int startTime the time a to enter the ready queue for a process.
*@param int id to distinguish each process.
*@param int runTime the total time a process has must run in the CPU.
*@param int exchangeTime[] array containing the process run time that it must make an exchange.
*@param int numExchange the total number of exchanges for a specific process.
*@param int totRunTime the current rime the process has been in the CPU.
*@param int exchangeCount used to index multiple exchage times for a process.
*@param int timeinR the time a process enters the ready queue.
*@param int totReadyTime the total time a process has spent in the ready queue.
*@param int timeinB the time a process enters the blocked queue.
*@param int totBlockedTime the total time a process has spent in the blocked queue and HDD.
**/
Process *createProcess(int startTime, int id, int runTime, int exchangeTime[], int numExchange, int totRunTime, int exchangeCount, int timeinR, int totReadyTime,int timeinB, int totBlockedTime);

/**Function returns startTime of process.
*@return the runTime associated with a process of type int.
*@param Process *p the information for process p.
**/
int startTime(Process *p);

/**Function returns ID of process.
*@return the ID associated with a process of type int.
*@param Process *p the information for process p.
**/
int id(Process *p);

/**Function returns runTime of process.
*@return the runTime associated with a process of type int.
*@param Process *p the information for process p.
**/
int runTime(Process *p);
/**Function returns exchangeTime of process.
*@return the exchangeTime associated with a process of type int.
*@param Process *p the information for process p.
*@param int i the index for an exchangeTime.
**/
int exchangeTime(Process *p, int i);
/**Function returns numExchange of process.
*@return the numExchange associated with a process of type int.
*@param Process *p the information for process p.
**/
int numExchange(Process *p);
/**Function returns totRunTime of process.
*@return the totRunTime associated with a process of type int.
*@param Process *p the information for process p.
**/
int totRunTime(Process *p);
/**Function returns exchangeCount of process.
*@return the exchageCount associated with a process of type int.
*@param Process *p the information for process p.
**/
int exchangeCount(Process *p);
/**Function returns timeinR of process.
*@return the timeinR associated with a process of type int.
*@param Process *p the information for process p.
**/
int timeinR(Process *p);
/**Function returns totReadyTime of process.
*@return the totReadyTime associated with a process of type int.
*@param Process *p the information for process p.
**/
int totReadyTime(Process *p);
/**Function returns timeinB of process.
*@return the timeinB associated with a process of type int.
*@param Process *p the information for process p.
**/
int timeinB(Process *p);
/**Function returns totBlockedTime of process.
*@return the totBlockedTime associated with a process of type int.
*@param Process *p the information for process p.
**/
int totBlockedTime(Process *p);


/**Function to delete process from the queue.
*@param void *tobeDeleted what is to be deleted from the queue.
**/
void deleteProcess(void *tobeDeleted);

/**Function to print out the process infromation.
*@param void * p the sepecific process that is to be printed.
**/
void printProcess(void * p);

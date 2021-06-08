/*
dispatcher.c

Student Name : Gurtej Dhami
Student ID # : 0969863

Dispatch Algorithm : Round Robin
*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100

#include<stdlib.h>
#include<string.h>
#include"linkedListAPI.h"
#include"process.h"
#include"queue.h"

struct cpu{
	int time;
};

struct hdd{
	int time;
};

void dispatcher(FILE *fd, int quantum, int harddrive){
	Queue *newQ = createQueue(printProcess, deleteProcess);
	Queue *readyQ = createQueue(printProcess, deleteProcess);
	Queue *waitCPU = createQueue(printProcess, deleteProcess);
	Queue *blockedQ = createQueue(printProcess, deleteProcess);
	Queue *waitBlocked = createQueue(printProcess, deleteProcess);
	Queue *toPrint = createQueue(printProcess, deleteProcess);
	Queue *P0 = createQueue(printProcess, deleteProcess);

    
    
	char line_buffer[MAX_LINE_LENGTH];
	int start_time, run_time, process_id, num_exchanges, exchange_time;
	char *token;
	int procCount = 0;
    
    //Process simulation input line by line
	while (fgets(line_buffer, MAX_LINE_LENGTH, fd) != NULL && line_buffer[0] != '\n'){
	        token = strtok(line_buffer, " ");
	        sscanf(token, "%d",&start_time);
	       
	        token = strtok(NULL, " ");
	        sscanf(token, "%d",&process_id);
	        
	        token = strtok(NULL, " ");
	        sscanf(token, "%d",&run_time);
        
	        num_exchanges = 0;
		int Exch[150] = {0};
		int k = 0;
		token = strtok(NULL, " ");
		while ( token != NULL ){
			num_exchanges += sscanf(token, "%d",&exchange_time);
			token = strtok(NULL, " ");
			Exch[k] = exchange_time;
			k++;
		}
		Process *p = createProcess(start_time, process_id, run_time, Exch,num_exchanges, 0, 0,0,0,0,0);
		push(newQ, p);
		procCount++;
	}
	int Quan = quantum;
	int hardDr = harddrive;
	int printChecker = 0;
	int totTime = 0;
	int isCpuEmpty = 0;
	int isBlockedEmpty = 0;
	int newProcGone = 0;
	int procReady = 0;
	int blocked = 0;
	int blockBeforeQuan = 0;
	int dontPrintYet = 0;
	int dummyArray[150] = {0};
	int exchSameasDone = 0;
	int ch = 0;
	struct cpu cpuT;
	struct hdd hddT;
	Process *p0 = createProcess(0, 0, 0, dummyArray,0, 0, 0,0,0,0,0);
	push(P0, p0);
	while(1){
		//only executes if the HDD is not empty
		if(isBlockedEmpty == 1){
			Process *inHDD = (Process *)top(waitBlocked);
			hddT.time = hddT.time - 1;
			 if(hddT.time == 0 ){
				inHDD->timeinR = totTime;
				inHDD->totBlockedTime = inHDD->totBlockedTime + totTime - inHDD->timeinB;
				Process *doneHDD = createProcess(inHDD->startTime, inHDD->id, inHDD->runTime, inHDD->exchangeTime,inHDD->numExchange, inHDD->totRunTime, inHDD->exchangeCounter, inHDD->timeinR, inHDD->totReadyTime, inHDD->timeinB, inHDD-> totBlockedTime);
				push(readyQ, doneHDD);
				procReady++;
				pop(waitBlocked);
				isBlockedEmpty = 0;
			}
			else{
				Process *stillinHDD = createProcess(inHDD->startTime, inHDD->id, inHDD->runTime, inHDD->exchangeTime,inHDD->numExchange, inHDD->totRunTime, inHDD->exchangeCounter, inHDD->timeinR, inHDD->totReadyTime, inHDD->timeinB, inHDD-> totBlockedTime);
				push(waitBlocked, stillinHDD);
				pop(waitBlocked);
			}
		}
		//only executes if the CPU is not empty
		if(isCpuEmpty == 1){
			Process *inCPU = (Process *)top(waitCPU);
			blockBeforeQuan = 0;
			dontPrintYet = 0;
			cpuT.time = cpuT.time - 1;
			inCPU->runTime = inCPU->runTime - 1;
			inCPU->totRunTime = inCPU->totRunTime + 1;
			//only executes if the current total run time of the process in the CPU is equal to the next exchange value 
			if(inCPU->exchangeTime[inCPU->exchangeCounter] == inCPU->totRunTime && inCPU->numExchange >= 0){
				//to ensure that this process enters Blocked before going into ready queue if quantum ends on same clock cycle
				if(cpuT.time == 0){
					blockBeforeQuan = 1;
				}
				//to ensure that this process enters Blocked before going into print queue if runTime ends on same clock cycle
				if(inCPU->runTime == 0){
					exchSameasDone++;
					if(inCPU->numExchange > 1){
						ch=1;
					}
					dontPrintYet = 1;
					inCPU->totRunTime = inCPU->totRunTime - 1;
					inCPU->runTime = inCPU->runTime + 1;
				}
				inCPU->exchangeCounter = inCPU->exchangeCounter + 1;
				inCPU->timeinB = totTime;
				Process *goingtoBlocked = createProcess(inCPU->startTime, inCPU->id, inCPU->runTime, inCPU->exchangeTime,inCPU->numExchange, inCPU->totRunTime, inCPU->exchangeCounter, inCPU->timeinR, inCPU->totReadyTime, inCPU->timeinB, inCPU-> totBlockedTime);
				push(blockedQ, goingtoBlocked);
				pop(waitCPU);
				blocked++;
				isCpuEmpty = 0;
				
			}
			//push the current process to the print queue if its runTime ends
			else if(inCPU->runTime == 0 && dontPrintYet == 0){
				Process *goingtoPrint = createProcess(inCPU->startTime, inCPU->id, inCPU->runTime, inCPU->exchangeTime,inCPU->numExchange, inCPU->totRunTime, inCPU->exchangeCounter, inCPU->timeinR, inCPU->totReadyTime, inCPU->timeinB, inCPU-> totBlockedTime); 
				push(toPrint, goingtoPrint);
				printChecker++;
				pop(waitCPU);
				isCpuEmpty = 0;
			}
			//push the current process to the ready queue if its quantum ends
			else if(cpuT.time == 0 && blockBeforeQuan == 0){
				inCPU->timeinR = totTime;
				Process *runTimeRemains = createProcess(inCPU->startTime, inCPU->id, inCPU->runTime, inCPU->exchangeTime,inCPU->numExchange, inCPU->totRunTime, inCPU->exchangeCounter, inCPU->timeinR, inCPU->totReadyTime, inCPU->timeinB, inCPU-> totBlockedTime);
				push(readyQ, runTimeRemains);
				procReady++;
				pop(waitCPU);
				isCpuEmpty = 0;
			}
			//push the current process back into the CPU queue and pop the previous process state before the current clock cycle
			else{
				Process *stillinCPU = createProcess(inCPU->startTime, inCPU->id, inCPU->runTime, inCPU->exchangeTime,inCPU->numExchange, inCPU->totRunTime, inCPU->exchangeCounter, inCPU->timeinR, inCPU->totReadyTime, inCPU->timeinB, inCPU-> totBlockedTime);
				push(waitCPU, stillinCPU);
				pop(waitCPU);
			}
			
		}
		//will print each process in order as they enter the queue along with process 0
		//will end the continuous loop
		if(printChecker == procCount){
			Process *pr0Print = (Process *)top(P0);
			if(ch == 1){
				pr0Print->totRunTime--;
			}
			printf("%d %d\n", pr0Print->id,pr0Print->totRunTime);
			for(int z = 0; z < procCount; z++){
				Process *print = (Process *)top(toPrint);
				printf("%d %d %d %d\n",print->id, print->totRunTime, print->totReadyTime, print->totBlockedTime);
				pop(toPrint);
			}
			break;
		}
		//if a process is ready to be put in the HDD and it is currently empty 
		//the process at the top of the blocked queue will enter the HDD  
		if(blocked > 0 && isBlockedEmpty == 0){
			Process *newB = (Process *)top(blockedQ);
			Process *readytoBlocked = createProcess(newB->startTime, newB->id, newB->runTime, newB->exchangeTime,newB->numExchange, newB->totRunTime, newB->exchangeCounter, newB->timeinR, newB->totReadyTime, newB->timeinB, newB-> totBlockedTime);
			push(waitBlocked, readytoBlocked);
			pop(blockedQ);
			hddT.time = hardDr;
			isBlockedEmpty = 1;
			blocked = blocked - 1;
		}
		//as long as all the processes have not left the new queue 
		//this will continue to check if the top process has the same start time as the current global clock
		if(newProcGone != procCount){		
			Process *newP = (Process *)top(newQ);
			if(newP->startTime == totTime){
				newP = (Process *)top(newQ);
				newP->timeinR = totTime;
				Process *readyPr = createProcess(newP->startTime, newP->id, newP->runTime, newP->exchangeTime,newP->numExchange, 0, 0, newP->timeinR, 0, 0, 0);
				newProcGone++;
				push(readyQ, readyPr);		
				pop(newQ);
				procReady= procReady+1;	
			}
		}	
		//if a process is ready to be put in the CPU and it is currently empty 
		//the process at the top of the ready queue will enter the CPU
		if(procReady > 0 && isCpuEmpty == 0){
			Process *newC = (Process *)top(readyQ);
			if(newC->timeinR != 0){
				newC->totReadyTime = newC->totReadyTime + totTime - newC->timeinR;
				newC->timeinR = 0;
			}
			Process *readytoCPU = createProcess(newC->startTime, newC->id, newC->runTime, newC->exchangeTime,newC->numExchange, newC->totRunTime, newC->exchangeCounter, newC->timeinR, newC->totReadyTime, newC->timeinB, newC-> totBlockedTime);
			push(waitCPU, readytoCPU);
			pop(readyQ);
			cpuT.time = Quan;
			isCpuEmpty = 1;
			procReady = procReady - 1;
			
		}
		//if the CPU us empty then the 0 process gets the CPU and totRunTime increments
		if(isCpuEmpty == 0){
			Process *pr0 = (Process *)top(P0);
			pr0->totRunTime = pr0->totRunTime + 1 + exchSameasDone;
			Process *readyPrint0 = createProcess(pr0->startTime, pr0->id, pr0->runTime, pr0->exchangeTime,pr0->numExchange, pr0->totRunTime, pr0->exchangeCounter, pr0->timeinR, pr0->totReadyTime, pr0->timeinB, pr0-> totBlockedTime);
			push(P0, readyPrint0);
			pop(P0);
			exchSameasDone = 0;
		}
		
		totTime++;
	}
}

#include "simulator.bbeaudin.h"
#include <stdio.h>
#include <stdlib.h>
#define RUN_TIME 4
//0 for FCFS, 1 for SJF, 2 for ROUND ROBIN
#define SCHED_TYPE 0
#define DEBUG 0
int queueProcess(PQueue *myQ, int priority, Process *process) {
  PQueueNode *newNode = (PQueueNode *) malloc(sizeof(PQueueNode));
  newNode->priority = priority;
  newNode->process = process;
  if (myQ->head ==  NULL) {
    newNode->next = NULL;
    myQ->head = newNode;
    myQ->tail = newNode;
    return(0);
  } else {
    PQueueNode *currentNode = myQ->head;
    PQueueNode *prevNode = NULL;
    while (currentNode != NULL && currentNode->priority <= priority) {
        prevNode = currentNode;
	currentNode = currentNode->next;
    }
    if (prevNode) {
	prevNode->next = newNode;
    } else {
        myQ->head = newNode;
    }
    newNode->next = currentNode;
    if (newNode->next == NULL) {
	myQ->tail = newNode;
    }
  }
  return(0);
}
int queueEvent(PQueue *myQ, int priority, Event *event) {
  PQueueNode *newNode = (PQueueNode *) malloc(sizeof(PQueueNode));
  newNode->priority = priority;
  newNode->event = event;
  if (myQ->head ==  NULL) {
    newNode->next = NULL;
    myQ->head = newNode;
    myQ->tail = newNode;
    return(0);
  } else {
    PQueueNode *currentNode = myQ->head;
    PQueueNode *prevNode = NULL;
    while (currentNode != NULL && currentNode->priority <= priority) {
        prevNode = currentNode;
	currentNode = currentNode->next;
    }
    if (prevNode) {
	prevNode->next = newNode;
    } else {
        myQ->head = newNode;
    }
    newNode->next = currentNode;
    if (newNode->next == NULL) {
	myQ->tail = newNode;
    }
  }
  return(0);
}

void * dequeue(PQueue *myQ) {
  PQueueNode *currentNode = myQ->head;
  Event *event = NULL;
  Process *process = NULL;
  if (currentNode != NULL) {
    event = currentNode->event;
    process = currentNode->process;
    myQ->head = currentNode->next;
    free(currentNode);
  }
  if (process == NULL){
  	return event;
  } else {
	return process;
  }
}

int printQueue(PQueue *eventQ, PQueue *cpuQ) {
  PQueueNode *currentEvent = eventQ->head;
  PQueueNode *currentReadyProcess = cpuQ->head;
  printf("---\neventQueue\n---\n");	
  while (currentEvent != NULL) {
    printf("| Priority:%d P %d | %s |\n", currentEvent->priority, currentEvent->event->process->pid, getEventTypeString(currentEvent->event->eventType));
    currentEvent = currentEvent->next;
  }
  printf("---\ncpuQueue\n---\n");	
  while (currentReadyProcess != NULL) {
    printf("| Priority:%d P %d burst:%d wait:%d |\n", currentReadyProcess->priority, currentReadyProcess->process->pid, currentReadyProcess->process->burstTime, currentReadyProcess->process->waitTime); 
    currentReadyProcess = currentReadyProcess->next;
  }	
  printf("------\n");	
  return(0);
}

void * peek(PQueue *myQ) {
  if(myQ->head != NULL) {
	return myQ->head->event;
  }
  return NULL;
}

int getMinPriority(PQueue *myQ) {
  int low = -1;
  if (myQ->head != NULL) 
    low = myQ->head->priority;
  return low;
}

void handleEvent(Event *myEvent, PQueue *eventQueue, PQueue *cpuQueue, int currentTime, int *idle) {	
	switch (myEvent->eventType) {
		case PROCESS_SUBMITTED:
			printf("t = %d , %s, pid = %d\n", currentTime, getEventTypeString(myEvent->eventType), myEvent->process->pid); 
			if (*idle) {
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_STARTS;
				newEvent->process = myEvent->process;
				queueEvent(eventQueue, currentTime, newEvent); 
				free(myEvent);
			} else {
				Process *readyProcess = myEvent->process;
				int priority;
				if (SCHED_TYPE == 1) {
					priority = readyProcess->burstTime;
				} else {
					priority = 0;
				}		
				queueProcess(cpuQueue, priority, readyProcess);
				free(myEvent);
			}
			break;
		case PROCESS_STARTS:
			printf("t = %d , %s, pid = %d\n", currentTime, getEventTypeString(myEvent->eventType), myEvent->process->pid); 
			if (SCHED_TYPE == 0 || SCHED_TYPE == 1) {
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_ENDS;
				newEvent->process = myEvent->process;
				newEvent->process->waitTime = currentTime + newEvent->process->submittedTime;
				queueEvent(eventQueue, currentTime + newEvent->process->burstTime, newEvent);
				free(myEvent);
				*idle = 0;
			} else {
 				/*if burstTime for this process > quantum then
				 create a new event PROCESS_TIMESLICE_EXPIRES at currentTime + quantum
				 else
				 create a new event event PROCESS_ENDS at currentTime + burstTime for this process*/
			}
			break;
		case PROCESS_ENDS:
			;//update stats about this process
			if (DEBUG) {
				printf("%s\n", getEventTypeString(myEvent->eventType));
				printf("%d\n", myEvent->process->waitTime);
				printf("%d\n", myEvent->process->pid);
			}
			printf("t = %d , %s, pid = %d, waitTime = %d\n", currentTime, getEventTypeString(myEvent->eventType), myEvent->process->pid, myEvent->process->waitTime); 
			Process *nextProcess;
			nextProcess = (peek(cpuQueue) == NULL) ? NULL : dequeue(cpuQueue);
			
			if (nextProcess != NULL) {
				if (DEBUG) {
					printf("queueing p%d at %d\n", nextProcess->pid, currentTime); 
				}
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_STARTS;
				newEvent->process = nextProcess;
				newEvent->process->pid = nextProcess->pid;
				newEvent->process->burstTime = nextProcess->burstTime;
				*idle = 1;
				queueEvent(eventQueue, currentTime, newEvent);
			}
			free(myEvent);
			break;
		/*case PROCESS_TIMESLICE_EXPIRES:
			printf("t = %d , PROCESS_TIMESLICE_EXPIRES, pid = %d\n", currentTime, myEvent->process->pid); 
			//update info for this process (subtract the quantum from the burstTime for this process)
			if (peek(cpuQueue) != NULL) {
				Process *expiredProcess;
				expiredProcess = dequeue(cpuQueue);
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_STARTS;
				newEvent->process = myEvent->process;
				queueEvent(eventQueue, currentTime, newEvent);
			} 
			break;*/
	}
}

char* getEventTypeString(EventType eventType) {
	switch (eventType) {
		case PROCESS_SUBMITTED: return "PROCESS_SUBMITTED";
		case PROCESS_STARTS: return "PROCESS_STARTS";
		case PROCESS_ENDS: return "PROCESS_ENDS";
		case PROCESS_TIMESLICE_EXPIRES: return "PROCESS_TIMESLICE_EXPIRES";
	}
}

int main() {
	int cpuIsIdle, currentTime;
	PQueue eventQueue;
	PQueue cpuQueue;
	Event *event;

	eventQueue.head = NULL;
	eventQueue.tail = NULL;
	cpuQueue.head = NULL;
	cpuQueue.tail = NULL;
	
	Process *p1 = (Process *) malloc(sizeof(Process));
	p1->pid = 1;
	p1->burstTime = 6;
	p1->waitTime = 0;
	Process *p2 = (Process *) malloc(sizeof(Process));
	p2->pid = 2;
	p2->burstTime = 7;
	p2->waitTime = 0;
	Process *p3 = (Process *) malloc(sizeof(Process));
	p3->pid = 3;
	p3->burstTime = 2;
	p3->waitTime = 0;
	Process *p4 = (Process *) malloc(sizeof(Process));
	p4->pid = 4;
	p4->burstTime = 5;
	p4->waitTime = 0;
	Process *p5 = (Process *) malloc(sizeof(Process));
	p5->pid = 5;
	p5->burstTime = 2;
	p5->waitTime = 0;
	//printf("Processes created\n");
	Event *e1 = (Event *) malloc(sizeof(Event));
	Event *e2 = (Event *) malloc(sizeof(Event));
	Event *e3 = (Event *) malloc(sizeof(Event));
	Event *e4 = (Event *) malloc(sizeof(Event));
	Event *e5 = (Event *) malloc(sizeof(Event));
	//printf("Events created\n");
	
	e1->eventType = PROCESS_SUBMITTED;
	e1->process = p1;
	queueEvent(&eventQueue, 0, e1);
	e2->eventType = PROCESS_SUBMITTED;
	e2->process = p2;
	queueEvent(&eventQueue, 3, e2);
	e3->eventType = PROCESS_SUBMITTED;
	e3->process = p3;
	queueEvent(&eventQueue, 4, e3);
	e4->eventType = PROCESS_SUBMITTED;
	e4->process = p4;
	queueEvent(&eventQueue, 6, e4);
	e5->eventType = PROCESS_SUBMITTED;
	e5->process = p5;
	queueEvent(&eventQueue, 6, e5);

	//printf("Events queued\n");
//	printQueue(&eventQueue, &cpuQueue);
	currentTime = getMinPriority(&eventQueue);
	cpuIsIdle = 1;	
	while (event != NULL) {
		if (DEBUG) {
			printQueue(&eventQueue, &cpuQueue);
			printf("handling event at %d, with Pid | type |\n", currentTime);//, event->process->pid);
		}
		handleEvent(event, &eventQueue, &cpuQueue, currentTime, &cpuIsIdle);	
		if (DEBUG) {
			printQueue(&eventQueue, &cpuQueue);
		}
//		printf("getting currentTime\n");
		currentTime = getMinPriority(&eventQueue);
//		printf("checking eventQueue\n");
		event = (peek(&eventQueue) == NULL) ? NULL :dequeue(&eventQueue);
		//printf(event);
	}

	return(0);
}

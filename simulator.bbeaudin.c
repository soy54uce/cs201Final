#include "simulator.bbeaudin.h"
#include <stdio.h>
#include <stdlib.h>
#define RUN_TIME 4
#define SCHED_TYPE 0

int enqueue(PQueue *myQ, int priority, DataNode *data) {
  PQueueNode *newNode = (PQueueNode *) malloc(sizeof(PQueueNode));
  newNode->priority = priority;
  newNode->data = data;
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

DataNode * dequeue(PQueue *myQ) {
  PQueueNode *currentNode = myQ->head;
  DataNode *data = NULL;
  if (currentNode != NULL) {
    data = currentNode->data;
    myQ->head = currentNode->next;
    free(currentNode);
  }
  return data;
}

int printQueue(PQueue *myQ) {
  PQueueNode *currentNode = myQ->head;
  while (currentNode != NULL) {
    //printf("| %d %s |\n", currentNode->priority, currentNode->data->event);
    currentNode = currentNode->next;
  }	
  return(0);
}

void * peek(PQueue *myQ) {
  if(myQ->head != NULL) {
	return myQ->head->data;
  }
  return NULL;
}

int getMinPriority(PQueue *myQ) {
  int low = -1;
  if (myQ->head != NULL) 
    low = myQ->head->priority;
  return low;
}

void handleEvent(Event *myEvent, PQueue *eventQueue, PQueue *cpuQueue, int currentTime, int *CPUisIdle) {	
		
	DataNode *data  = (DataNode *) malloc(sizeof(Event));
	switch (myEvent->eventType) {
		case PROCESS_SUBMITTED:
			printf("t = %d , PROCESS_SUBMITTED, pid = %d\n", currentTime, myEvent->process->pid); 
			if (CPUisIdle) {
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_STARTS;
				newEvent->process = myEvent->process;
				data->event = newEvent;
				enqueue(eventQueue, currentTime, data); 
			} else {
				Process *readyProcess = myEvent->process;
				int priority;
				priority = 0;
				if (SCHED_TYPE == 2) {
					priority = readyProcess->burstTime;
				}
				data->process = readyProcess;
				enqueue(cpuQueue, priority, data);
			}
			break;
		case PROCESS_STARTS:
			printf("t = %d , PROCESS_STARTS, pid = %d\n", currentTime, myEvent->process->pid); 
			if (SCHED_TYPE == 0 || SCHED_TYPE == 1) {
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_ENDS;
				newEvent->process = myEvent->process;
				data->event = newEvent;
				enqueue(eventQueue, currentTime + myEvent->process->burstTime, data);
			} else {
 				/*if burstTime for this process > quantum then
				 create a new event PROCESS_TIMESLICE_EXPIRES at currentTime + quantum
				 else
				 create a new event event PROCESS_ENDS at currentTime + burstTime for this process*/
			}
			break;
		case PROCESS_ENDS:
			printf("t = %d , PROCESS_ENDS, pid = %d\n", currentTime, myEvent->process->pid); 
			//update stats about this process
			if (peek(cpuQueue) != NULL) {
				Process *endingProcess;
				endingProcess =  dequeue(cpuQueue)->process;
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_STARTS;
				newEvent->process = myEvent->process;
				data->event = newEvent;
				enqueue(eventQueue, currentTime, data);
			}
			break;
		case PROCESS_TIMESLICE_EXPIRES:
			printf("t = %d , PROCESS_TIMESLICE_EXPIRES, pid = %d\n", currentTime, myEvent->process->pid); 
			//update info for this process (subtract the quantum from the burstTime for this process)
			if (peek(cpuQueue) != NULL) {
				Process *expiredProcess;
				expiredProcess = dequeue(cpuQueue)->process;
				Event *newEvent = (Event *) malloc(sizeof(Event));
				newEvent->eventType = PROCESS_STARTS;
				newEvent->process = myEvent->process;
				data->event = newEvent;
				enqueue(eventQueue, currentTime, data);
			} 
			break;
	}
}

int main() {
	int cpuIsIdle, currentTime;
	PQueue eventQueue;
	PQueue cpuQueue;
	DataNode *data = (DataNode *) malloc(sizeof(DataNode));
	Event *event;

	eventQueue.head = NULL;
	eventQueue.tail = NULL;
	cpuQueue.head = NULL;
	cpuQueue.tail = NULL;
	
	Process *p1 = (Process *) malloc(sizeof(Event));
	p1->pid = 1;
	p1->burstTime = 6;
	p1->waitTime = 0;
	Process *p2 = (Process *) malloc(sizeof(Event));
	p2->pid = 2;
	p2->burstTime = 7;
	p2->waitTime = 0;
	Process *p3 = (Process *) malloc(sizeof(Event));
	p3->pid = 3;
	p3->burstTime = 2;
	p3->waitTime = 0;
	Process *p4 = (Process *) malloc(sizeof(Event));
	p4->pid = 4;
	p4->burstTime = 5;
	p4->waitTime = 0;
	Process *p5 = (Process *) malloc(sizeof(Event));
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
	e2->eventType = PROCESS_SUBMITTED;
	e2->process = p2;
	e3->eventType = PROCESS_SUBMITTED;
	e3->process = p3;
	e4->eventType = PROCESS_SUBMITTED;
	e4->process = p4;
	e5->eventType = PROCESS_SUBMITTED;
	e5->process = p5;

	data->event = e1;
	enqueue(&eventQueue, 1, data);
	data->event = e2;
	enqueue(&eventQueue, 2, data);
	data->event = e3;
	enqueue(&eventQueue, 3, data);
	data->event = e4;
	enqueue(&eventQueue, 4, data);
	data->event = e5;
	enqueue(&eventQueue, 5, data);
	//printf("Events queued\n");
	
	currentTime = getMinPriority(&eventQueue);
	printf("Handling Event %d\n", currentTime);
	event =  dequeue(&eventQueue)->event;
	while (event != NULL) {
	//	printf("looping\n");
		handleEvent(event, &eventQueue, &cpuQueue, currentTime, &cpuIsIdle);	
		currentTime = getMinPriority(&eventQueue);
		printf("Handling Event %d\n", event->process->pid);
		event = (peek(&eventQueue) == NULL) ? NULL :dequeue(&eventQueue)->event;
	}

	return(0);
}

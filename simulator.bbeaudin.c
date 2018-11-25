#include "simulator.bbeaudin.h"
#include <stdio.h>
#include <stdlib.h>
#define TIME_QUANTUM 4

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
    printf("| %d %s |\n", currentNode->priority, currentNode->data->name);
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

void handleEvent(Event myEvent) {	
	switch (myEvent->eventType) {
		case PROCESS_SUBMITTED:
			if (cpuIsIdle) {
				Event *nextEvent = (Event *) malloc(sizeof(Event));
			}
			break;
		case PROCESS_STARTS:
			break;
		case PROCESS_ENDS:
			break:
		case PROCESS_TIMESLICE_EXPIRES:
			break;
}

int main() {
	int cpuIdIdle;
	PQueue eventQueue;
	PQueue cpuQueue;
	DataNode *data;

	eventQueue.head = NULL;
	eventQueue.tail = NULL;
	cpuQueue.head = NULL;
	cpuQueue.tail = NULL;
	
	Process *p1 = (Process *) malloc(sizeof(Event));
	Process *p2 = (Process *) malloc(sizeof(Event));
	Process *p3 = (Process *) malloc(sizeof(Event));
	Process *p4 = (Process *) malloc(sizeof(Event));
	Process *p5 = (Process *) malloc(sizeof(Event));
	
	Event *e1 = (Event *) malloc(sizeof(Event));
	Event *e2 = (Event *) malloc(sizeof(Event));
	Event *e3 = (Event *) malloc(sizeof(Event));
	Event *e4 = (Event *) malloc(sizeof(Event));
	Event *e5 = (Event *) malloc(sizeof(Event));
	e1->eventType = PROCESS_SUBMITTED;
	e1->process = p1;
	e2->eventType = PROCESS_SUBMITTED;
	e2->process = p1;
	e3->eventType = PROCESS_SUBMITTED;
	e3->process = p1;
	e4->eventType = PROCESS_SUBMITTED;
	e4->process = p1;
	e5->eventType = PROCESS_SUBMITTED;
	e5->process = p1;
	data->event = e1;
	enqueue(&eventQueue, 1, data);
	data->event = e2;
	enqueue(&eventQueue, 2, e2);
	data->event = e3;
	enqueue(&eventQueue, 3, e3);
	data->event = e4;
	enqueue(&eventQueue, 4, e4);
	data->event = e5;
	enqueue(&eventQueue, 5, e5);
	
	currentTime = getMinPriority(eventQueue);
	event = dequeue(eventQueue);
	while (event != NULL && currentTime < RUN_TIME) {
		handleEvent(event);
		currentTime = getMinPriority(eventQueue);
		event = dequeue(eventQueue);
	}

	return(0);
}

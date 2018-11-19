#include "pqueue.bbeaudin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void * dequeue(PQueue *myQ) {
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
  if (currentNode != NULL) {
    printf("|");
  }
  while (currentNode != NULL) {
    printf(" %d %s |", currentNode->priority, currentNode->data->name);
    currentNode = currentNode->next;
  }
  printf("\n");
  return(0);
}

void * peek(PQueue *myQ) {
  if (myQ->head != NULL) {
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

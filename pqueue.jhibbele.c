#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "pqueue.jhibbele.h"

/*--------------------------------------------------------------------*/
/* insert at tail */

int enqueue(PQueue *pqueue, int priority, void *data) {
  PQueueNode *queueNode, *newNode, *prevNode;

  newNode = (PQueueNode *) malloc(sizeof(PQueueNode));
  newNode->next = NULL;
  newNode->priority = priority;
  newNode->data = data;

  if (pqueue->head == NULL) {
    pqueue->head = newNode;
    pqueue->tail = newNode;
  } else {
    if (priority < pqueue->head->priority) {
      /* insert at front */
      newNode->next = pqueue->head;
      pqueue->head = newNode;
    } else if (priority >= pqueue->tail->priority) {
      /* insert at end */
      pqueue->tail->next = newNode;
      pqueue->tail = newNode;
    } else {
      prevNode = NULL;
      queueNode = pqueue->head;
      while (queueNode != pqueue->tail && priority >= queueNode->priority) {
        prevNode = queueNode;
        queueNode = queueNode->next;
      }

      assert(prevNode != NULL);

      /* insert in middle */
      prevNode->next = newNode;
      newNode->next = queueNode;
    }
  }
  return(0);
}

/*--------------------------------------------------------------------*/

int printQueue(PQueue *pqueue) {
  PQueueNode *queueNode;
  DataNode *dataNode;

  printf("|");
  queueNode = pqueue->head;
  while (queueNode != NULL) {
    dataNode = (DataNode *) queueNode->data;
    printf("%d %s|", queueNode->priority, dataNode->name);
    queueNode = queueNode->next;
  }
  if (pqueue->head == NULL)
    printf("|");
  printf("\n");
  return(0);
}

/*--------------------------------------------------------------------*/
/* remove from head */

void *dequeue(PQueue *pqueue) {
  PQueueNode *queueNode;
  void *rtnval;

  if (pqueue->head == NULL) {
    return(NULL);
  } else {
    queueNode = pqueue->head;
    pqueue->head = queueNode->next;
    if (pqueue->head == NULL)
      pqueue->tail = NULL;
    //queueNode->next = NULL;
    rtnval = queueNode->data;
    free(queueNode);
    return(rtnval);
  }
}

/*--------------------------------------------------------------------*/
/* peek at head */

void *peek(PQueue *pqueue) {
  if (pqueue->head != NULL)
    return(pqueue->head->data);
  else
    return(NULL);
}

//--------------------------------------------------------------------

int getMinPriority(PQueue *pqueue) {
  if (pqueue->head != NULL)
    return(pqueue->head->priority);
  else
    return(-1);
}

//--------------------------------------------------------------------

int queueLength(PQueue *pqueue) {
  PQueueNode *queueNode;
  int len;
  len = 0;
  queueNode = pqueue->head;
  while (queueNode != NULL) {
    ++len;
    queueNode = queueNode->next;
  }
  return(len);
}


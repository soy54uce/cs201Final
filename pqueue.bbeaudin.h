typedef struct {
  char name[16];
} DataNode;

typedef struct PQN {
  int priority;
  DataNode *data;
  struct PQN *next;
} PQueueNode;

typedef struct {
  PQueueNode *head;
  PQueueNode *tail;
} PQueue;

int enqueue(PQueue *pqueue, int priority, DataNode *data);
// put node in quese in priority order; in case of tie in priority,
// then put node after all nodes with the specified priority

void * dequeue(PQueue *pqueue);
// return NULL is queue is empty; otherwise return first node in queue

int printQueue(PQueue *pqueue);
// print queue from head to tail

void * peek(PQueue *pqueue);
// return the first node in the queue, but do not remove it; return NULL
// if queue is empty

int getMinPriority(PQueue *pqueue);
// return priority of the first node in queue; return -1 if queue is empty

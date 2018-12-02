typedef struct {
  char name[16];
} DataNode;

typedef struct PQN {
  int priority;
  void *data;
  struct PQN *next;
} PQueueNode;

typedef struct {
  PQueueNode *head;
  PQueueNode *tail;
} PQueue;

/*--------------------------------------------------------------------*/
/* insert at tail */

int enqueue(PQueue *pqueue, int priority, void *data);

/*--------------------------------------------------------------------*/

int printQueue(PQueue *pqueue);

/*--------------------------------------------------------------------*/
/* remove from head */

void *dequeue(PQueue *pqueue);

/*--------------------------------------------------------------------*/
/* peek at head node */

void *peek(PQueue *pqueue);

/*--------------------------------------------------------------------*/
/* get priority of head node */

int getMinPriority(PQueue *pqueue);

/*--------------------------------------------------------------------*/

int queueLength(PQueue *pqueue);

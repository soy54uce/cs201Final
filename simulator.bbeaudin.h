typedef enum EventTypeEnum {
 PROCESS_SUBMITTED,
 PROCESS_STARTS,
 PROCESS_ENDS,
 PROCESS_TIMESLICE_EXPIRES
} EventType;

typedef struct {
 int pid;
 int burstTime;
 int submittedTime;
 int waitTime;
 int numPreemptions;
 int lastTime;
} Process;

typedef struct {
 EventType eventType;
 Process *process;
} Event;

typedef struct PQN {
  int priority;
  Event *event;
  Process *process;
  struct PQN *next;
} PQueueNode;

typedef struct {
  PQueueNode *head;
  PQueueNode *tail;
} PQueue;


int queueEvent(PQueue *pqueue, int priority, Event *event);
// put node in queue in priority order; in case of tie in priority,
// then put node after all nodes with the specified priority
int queueProcess(PQueue *pqueue, int priority, Process *process);

void * dequeue(PQueue *pqueue);
// return NULL is queue is empty; otherwise return first node in queue

int printQueue(PQueue *pqueue, PQueue *cpuQueue);
// print queue from head to tail

void * peek(PQueue *pqueue);
// return the first node in the queue, but do not remove it; return NULL
// if queue is empty

int getMinPriority(PQueue *pqueue);
// return priority of the first node in queue; return -1 if queue is empty

// check event type and process accordingly
void handleEvent(Event *myEvent, PQueue *eventQueue, PQueue *cpuQueue, int currentTime, int *CPUisIdle);	

// initialize all of the events
void eventInit();

char* getEventTypeString(EventType eventType);

void printEvent(Event anyEvent);
void printProcess(Process anyProcess);

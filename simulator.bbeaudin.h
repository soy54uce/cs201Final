typedef struct {
	int pid;
	int burstTime;
	int waitTime;
	int numPreemtions;
	int lastTime;
} Process;

typedef struct {
	EventType eventType;
	Process *process;
} Event;

typedef enum EventTypeEnum {
	PROCESS_SUBMITTED,
	PROCESS_STARTS,
	PROCESS_ENDS,
	PROCESS_TIMESLICE_EXPIRES
} EventType;

// check event type and process accordingly
void handleEvent(Event event);

// initialize all of the events
void eventInit();

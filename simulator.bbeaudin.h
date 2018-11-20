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

void handleEvent(Event event);



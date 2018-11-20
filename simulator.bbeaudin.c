#include "pqueue.bbeaudin.h"
#include "simulator.bbeaudin.h"
#include <stdio.h>
#include <stdlib.h>
#define TIME_QUANTUM 4

int cpuIdIdle;
PQueue eventQueue;
PQueue cpuQueue;

void handleEvent(Event event) {	
	switch (event.eventType) {
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
	

	currentTime = getMinPriority(eventQueue);
	event = dequeue(eventQueue);
	while (event != NULL && currentTime < RUN_TIME) {
		handleEvent(event);
		currentTime = getMinPriority(eventQueue);
		event = dequeue(eventQueue);
	}

	return(0);
}

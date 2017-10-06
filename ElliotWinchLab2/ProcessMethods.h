#include <stdio.h>

#define UNSTARTED 0
#define READY 1
#define BLOCKED 2
#define RUNNING 3
#define TERMINATED 4

typedef struct ProcessData{
	//READ ONLY
	int A; //arrival time
	int B; //for use in the random IO number calculation
	int C; //total CPU time needed
	int M; //IO time multiplier

	//FOR OUTPUT PURPOSES
	int CPUTime;
	int IOTime;
	int waitTime;


	int currentWaitTime;
	int state; //unstarted = 0, ready = 1, blocked = 2, running = 3, terminated = 4
	int blockedTimeRemaining;
	int currentCPUBurstTime;
	int CPUBurstTimeRemaining;
	int totalCPUTimeRemaining;

	//for RR only
	int quantum;
} ProcessData;

int unFinishedProcesses(ProcessData* processes[], int numProcesses);
int checkBlockedProcesses(ProcessData* processes[], int numProcesses);
void checkArrivingProcesses(ProcessData* processes[], int numProcesses, int timeCounter);
void runProcess(ProcessData* currentProcess);
void incReadyProcesses(ProcessData* processes[], int numProcesses);
int randomOS(int B, FILE* randNumFile);
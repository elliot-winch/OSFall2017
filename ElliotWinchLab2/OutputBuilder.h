#include <stdlib.h>
#include <stdio.h>

#define UNSTARTED 0
#define READY 1
#define BLOCKED 2
#define RUNNING 3
#define TERMINATED 4

#define MAX_RAND_INPUT_LENGTH 64

typedef struct ProcessData
{
	//
	int A; //arrival time
	int B; //for use in the random IO number calculation
	int C; //total CPU time needed
	int M; //IO time multiplier

	//READ AND WRITE
	int CPUTime;
	int IOTime;
	int waitTime;
	int currentWaitTime;

	int state; //unstarted = 0, ready = 1, blocked = 2, running = 3, terminated = 4
	int blockedTimeRemaining;
	int currentCPUBurstTime;
	int CPUBurstTimeRemaining;
	int totalCPUTimeRemaining;

} ProcessData;

void originalInput(ProcessData* processes[], int numProcesses);
void processFinishedOutput(ProcessData* processes[], int numProcesses);
void summaryData(ProcessData* processes[], int numProcesses, int finishingTime, int CPUUseTime, int IOUseTime);
int verboseData(ProcessData* processes[], int numProcesses);
char* getStateString(int state);
int randomOS(int B, FILE* randNumFile);
void printProcess(ProcessData* process);
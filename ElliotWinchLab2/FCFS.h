#include <stdio.h>

typedef struct ProcessData ProcessData;

void FCFS(ProcessData* processes[], int numProcesses, int verboseMode, FILE* randNumFile);
int unFinishedProcesses(ProcessData* processes[], int numProcesses);
int checkBlockedProcesses(ProcessData* processes[], int numProcesses);
void checkArrivingProcesses(ProcessData* processes[], int numProcesses, int timeCounter);
void runProcess(ProcessData* currentProcess);
void incReadyProcesses(ProcessData* processes[], int numProcesses);
int findProcess(ProcessData* processes[], int numProcesses, int* currentProcessIndex);
#include <stdlib.h>
#include <stdio.h>
#include "ProcessMethods.h"

#define MAX_RAND_INPUT_LENGTH 64

typedef struct ProcessData ProcessData;

void printInput(ProcessData* processes[], int numProcesses);
void processFinishedOutput(ProcessData* processes[], int numProcesses);
void summaryData(ProcessData* processes[], int numProcesses, int finishingTime, int CPUUseTime, int IOUseTime);
int verboseData(ProcessData* processes[], int numProcesses);
char* getStateString(int state);
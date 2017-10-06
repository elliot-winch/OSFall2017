#include <stdio.h>

typedef struct ProcessData ProcessData;

void FCFS(ProcessData* processes[], int numProcesses, int verboseMode, FILE* randNumFile);
int findProcessFCFS(ProcessData* processes[], int numProcesses, int* currentProcessIndex);
#include <stdio.h>

typedef struct ProcessData ProcessData;

void RR(ProcessData* processes[], int numProcesses, int verboseMode, FILE * randNumFile);
int findProcessRR(ProcessData* processes[], int numProcesses, int* currentProcessIndex);
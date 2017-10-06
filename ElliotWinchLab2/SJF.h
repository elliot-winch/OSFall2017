#include <stdio.h>

typedef struct ProcessData ProcessData;

void SJF(ProcessData* processes[], int numProcesses, int verboseMode, FILE * randNumFile);
int findProcessSJF(ProcessData* processes[], int numProcesses, int* currentProcessIndex);
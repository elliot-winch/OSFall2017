#include <stdio.h>

typedef struct ProcessData ProcessData;

void HPRN(ProcessData* processes[], int numProcesses, int verboseMode, FILE* randNumFile);
int findProcessHPRN(ProcessData* processes[], int numProcesses, int* currentProcessIndex);
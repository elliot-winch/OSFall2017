#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "OutputBuilder.h"
#include "FCFS.h"

// #include "RR.c"
// #include "SJF.c"
// #include "HPRN.c"


int main(int argc, char *argsv[]){

	if((argc > 3 || argc == 1)){
		printf("Incorrect number of arguments.\n");
		printf("Usage: ./a.out [--verbose] processFilePath\n");
		return;
	} else if (argc ==3 || strcmp(argsv[2],"--verbose")){
		printf("Invalid flag.\n");
		printf("Usage: ./a.out [--verbose] processFilePath\n");

	}

	FILE * processFile;
	processFile = fopen(argsv[argc - 1], "r");

	if(processFile == NULL){
		printf("Error: Cannot find file called %s\n", argsv[argc - 1]);
		return;
	}

	FILE * randNumFile = fopen("random-numbers", "r");

	int numProcesses = 0;
	fscanf(processFile, "%d", &numProcesses);
	
	ProcessData* processesAsRead[numProcesses];
	ProcessData* processes[numProcesses];

	readProcesses(numProcesses, processesAsRead, processFile);

	originalInput(processesAsRead, numProcesses);

	/*prepProcessData(processesAsRead, processes, numProcesses);
	FCFS(processes, numProcesses, argc - 2, randNumFile);
	
	prepProcessData(processesAsRead, processes, numProcesses);
	RR(processes, numProcesses, argc - 2, randNumFile);
		

	prepProcessData(processesAsRead, processes, numProcesses);
	SJF(processes, numProcesses, argc - 2, randNumFile);
	*/

	prepProcessData(processesAsRead, processes, numProcesses);
	HPRN(processes, numProcesses, argc - 2, randNumFile);

	fclose(processFile);
	fclose(randNumFile);
	
}

int readProcesses(int numProcesses, ProcessData* processes[], FILE* processFile){

	int i;
	for(i = 0; i < numProcesses; i++){
		processes[i] = malloc(sizeof(ProcessData));
		fscanf(processFile, "%d %d %d %d", &(processes[i]->A), &(processes[i]->B), &(processes[i]->C), &(processes[i]->M));
	}
}

void prepProcessData(ProcessData* src[], ProcessData* dest[], int numProcesses){

	int i;
	for(i = 0; i < numProcesses; i++){
		dest[i] = malloc(sizeof(ProcessData));

		dest[i]->A = src[i]->A;
		dest[i]->B = src[i]->B;
		dest[i]->C = src[i]->C;
		dest[i]->M = src[i]->M;

		dest[i]->CPUTime = 0;
		dest[i]->IOTime = 0;
		dest[i]->waitTime = 0;
		dest[i]->currentWaitTime = 0;

		dest[i]->state = 0;
		dest[i]->blockedTimeRemaining = 0;
		dest[i]->currentCPUBurstTime = 0;
		dest[i]->CPUBurstTimeRemaining = 0;
		dest[i]->totalCPUTimeRemaining = src[i]->C;
	}
}
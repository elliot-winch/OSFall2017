#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "OutputBuilder.h"
#include "FCFS.h"

// #include "RR.c"
// #include "SJF.c"
// #include "HPRN.c"


int main(int argc, char *argsv[]){

	if(argc > 3 || argc == 1){
		printf("Usage: ./a.out [--verbose] processFilePath\n");
		return;
	}

	FILE * randNumFile = fopen("random-numbers", "r");

	FILE * processFile;

	//Deal with verbose flag here
	int verbose;
	if(argc == 3){
		processFile = fopen(argsv[2], "r");
		verbose = 1;
	} else {
		processFile = fopen(argsv[1], "r");
		verbose = 0;
	}

	if(processFile == NULL){
		printf("Error: Cannot find file\n");
		return;
	}

	int numProcesses = 0;
	fscanf(processFile, "%d", &numProcesses);
	
	ProcessData* processes[numProcesses];

	readProcesses(numProcesses, processes, processFile);

	originalInput(processes, numProcesses);

	FCFS(processes, numProcesses, verbose, randNumFile);

	fclose(processFile);
	fclose(randNumFile);
	
}

int readProcesses(int numProcesses, ProcessData** processes, FILE* processFile){

	int i;
	for(i = 0; i < numProcesses; i++){
		processes[i] = malloc(sizeof(ProcessData));
		fscanf(processFile, "%d %d %d %d", &(processes[i]->A), &(processes[i]->B), &(processes[i]->C), &(processes[i]->M));
		
		processes[i]->CPUTime = 0;
		processes[i]->IOTime = 0;
		processes[i]->waitTime = 0;
		processes[i]->currentWaitTime = 0;

		processes[i]->state = 0;
		processes[i]->blockedTimeRemaining = 0;
		processes[i]->currentCPUBurstTime = 0;
		processes[i]->CPUBurstTimeRemaining = 0;
		processes[i]->totalCPUTimeRemaining = processes[i]->C;
	}
}
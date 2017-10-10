#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "OutputBuilder.h"
#include "FCFS.h"
#include "RR.h"
#include "SJF.h"
#include "HPRN.h"


int main(int argc, char *argsv[]){

	if((argc > 3 || argc == 1)){
		printf("Incorrect number of arguments.\n");
		printf("Usage: ./a.out [--verbose] processFilePath\n");
		exit(1);
	} else if (argc == 3 && !strcmp(argsv[2],"--verbose")){
		printf("Invalid flag.\n");
		printf("Usage: ./a.out [--verbose] processFilePath\n");
		exit(1);
	}

	FILE * processFile;
	processFile = fopen(argsv[argc - 1], "r");

	if(processFile == NULL){
		printf("Error: Cannot find file called %s\n", argsv[argc - 1]);
		exit(1);
	}

	FILE* randNumFile = fopen("random-numbers", "r");

	if(randNumFile == NULL){
		printf("Error: Cannot find random-numbers file\n");
		exit(1);
	}

	int numProcesses = 0;
	fscanf(processFile, "%d", &numProcesses);
	
	ProcessData* processesAsRead[numProcesses];
	ProcessData* processes[numProcesses];

	readProcesses(numProcesses, processesAsRead, processFile);

	prepProcessData(processesAsRead, processes, numProcesses);
	FCFS(processes, numProcesses, argc - 2, randNumFile);

	prepProcessData(processesAsRead, processes, numProcesses);
	/*Be kind*/rewind(randNumFile);
	RR(processes, numProcesses, argc - 2, randNumFile);
	
	prepProcessData(processesAsRead, processes, numProcesses);
	rewind(randNumFile);
	SJF(processes, numProcesses, argc - 2, randNumFile);

	prepProcessData(processesAsRead, processes, numProcesses);
	rewind(randNumFile);
	HPRN(processes, numProcesses, argc - 2, randNumFile);

	//free processesAsRead and processes
	int f;
	for(f = 0; f < numProcesses; f++){
		free(processes[f]);
		free(processesAsRead[f]);
	}

	fclose(processFile);
	fclose(randNumFile);
	
	return 0;
}

int readProcesses(int numProcesses, ProcessData* processes[], FILE* processFile){

	int i;
	for(i = 0; i < numProcesses; i++){
		processes[i] = malloc(sizeof(ProcessData));
		fscanf(processFile, "%d %d %d %d", &(processes[i]->A), &(processes[i]->B), &(processes[i]->C), &(processes[i]->M));
	}

	printf("The original input was:\t %d ", numProcesses);

	printInput(processes, numProcesses);

	//Insertion sort. Though it is O(n^2), for
	//sorting less than ten elements the difference between
	//O(n^2) and O(logn) is minimal.
	int k, j;
	ProcessData* key;
	for(k = 0; k < numProcesses; k++){
		key = processes[k];
		j = k - 1;

		while(j >= 0 && processes[j]->A > key->A){
			processes[j+1] = processes[j];
			j--;
		}
		processes[j+1] = key;
	}

	printf("The sorted input was:\t %d ", numProcesses);

	printInput(processes, numProcesses);
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
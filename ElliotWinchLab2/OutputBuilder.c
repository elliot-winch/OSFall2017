#include "OutputBuilder.h"

void originalInput(ProcessData* processes[], int numProcesses){

	printf("\n\nThe original input was: %d", numProcesses);

	int i;
	for(i = 0; i < numProcesses; i++){
		printf("(%d %d %d %d) ", processes[i]->A, processes[i]->B, processes[i]->C, processes[i]->M);
	}

	printf("\n");
}

void processFinishedOutput(ProcessData* processes[], int numProcesses){

	printf("\n\n");
	int i;
	for(i = 0; i < numProcesses; i++){
		printf("Process %d:\n", i);
		printf("(A,B,C,M) = (%d %d %d %d)\n", processes[i]->A, processes[i]->B, processes[i]->C, processes[i]->M);

		int turnaroundTime =  processes[i]->CPUTime + processes[i]->IOTime + processes[i]->waitTime;
		printf("\tFinishing Time: %d\n", processes[i]->A + turnaroundTime);
		printf("\tTurnaround Time: %d\n", turnaroundTime);
		printf("\tIO Time: %d\n", processes[i]->IOTime);
		printf("\tWait Time: %d\n", processes[i]->waitTime);
		printf("\n");
	}
}

void summaryData(ProcessData* processes[], int numProcesses, int finishingTime, int CPUUseTime, int IOUseTime){

	printf("Summary Data:\n");
	printf("\tFinshing time: %d\n", finishingTime);
	printf("\tCPU Utilization: %f\n", CPUUseTime/(float)finishingTime);
	printf("\tI/O Utilization: %f\n", IOUseTime/(float)finishingTime);
	printf("\tThroughput: %f processes per hundred cycles\n", (numProcesses/(float)finishingTime)*100);

	float averageTurnaround = 0;
	int i;
	for (i = 0; i < numProcesses; ++i)
	{
		averageTurnaround += processes[i]->CPUTime + processes[i]->IOTime + processes[i]->waitTime;
	}
	averageTurnaround = averageTurnaround / numProcesses;

	printf("\tAverage Turnaround Time: %f\n", averageTurnaround);

	float averageWait = 0;
	for (i = 0; i < numProcesses; ++i)
	{
		averageWait += processes[i]->waitTime;
	}
	averageWait = averageWait / (float)numProcesses;

	printf("\tAverage Wait Time: %f\n", averageWait);

	printf("\n\n");
}

int verboseData(ProcessData* processes[], int numProcesses){
	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == BLOCKED){
			printf("%s %d ", getStateString(processes[i]->state), processes[i]->blockedTimeRemaining);
		} else if(processes[i]->state == RUNNING){
			printf("%s %d ", getStateString(processes[i]->state), processes[i]->CPUBurstTimeRemaining);
		} else {
			printf("%s 0 ", getStateString(processes[i]->state));
		}
	}
	printf("\n");
}

char* getStateString(int state){

	switch(state){
		case 0: return "unstarted";
		case 1: return "ready";
		case 2: return "blocked";
		case 3: return "running";
		case 4: return "terminated";
		default: return "Unknown state!";
	}
}
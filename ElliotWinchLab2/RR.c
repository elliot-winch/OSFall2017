#include "RR.h"
#include "OutputBuilder.h"

#define QUANTUM 2

void RR(ProcessData* processes[], int numProcesses, int verboseMode, FILE * randNumFile){

	int timeCounter = -1;
	int CPUUseTime = 0;
	int IOUseTime = 0;

	int currentProcessIndex = -1;

	int i;
	for(i = 0; i < numProcesses; i++){
		processes[i]->quantum = 0;
	}

	while(unFinishedProcesses(processes, numProcesses)){

		timeCounter++;

		if(verboseMode){
			printf("Before cycle %d:\t", timeCounter);
			verboseData(processes, numProcesses);
		}

		IOUseTime += checkBlockedProcesses(processes, numProcesses);
		checkArrivingProcesses(processes, numProcesses, timeCounter);

		if(currentProcessIndex >= 0){

			runProcess(processes[currentProcessIndex]);
			processes[currentProcessIndex]->quantum--;
			CPUUseTime++;

			if(processes[currentProcessIndex]->totalCPUTimeRemaining <= 0){
				processes[currentProcessIndex]->state = TERMINATED;
				currentProcessIndex = -1;
			} else if(processes[currentProcessIndex]->CPUBurstTimeRemaining <= 0){
				processes[currentProcessIndex]->state = BLOCKED;
				processes[currentProcessIndex]->blockedTimeRemaining = processes[currentProcessIndex]->currentCPUBurstTime * processes[currentProcessIndex]->M;
				currentProcessIndex = -1;
			} else if(processes[currentProcessIndex]->quantum <= 0){
				processes[currentProcessIndex]->state = READY;

				// processes[currentProcessIndex]->currentWaitTime = 0;
				currentProcessIndex = -1;
			}
		}

		if(currentProcessIndex == -1){
			if(findProcessRR(processes, numProcesses, &currentProcessIndex)){
				if(processes[currentProcessIndex]->CPUBurstTimeRemaining <= 0){
				int burstTime = randomOS(processes[currentProcessIndex]->B, randNumFile);

					processes[currentProcessIndex]->currentCPUBurstTime = burstTime;
					processes[currentProcessIndex]->CPUBurstTimeRemaining = burstTime;
				}

				processes[currentProcessIndex]->quantum = QUANTUM;

				processes[currentProcessIndex]->state = RUNNING;
				processes[currentProcessIndex]->currentWaitTime = 0;
			}
		}

		incReadyProcesses(processes, numProcesses);
	}

	printf("\nThe scheduling algorithm used was Round Robin.\n");

	processFinishedOutput(processes, numProcesses);

	summaryData(processes, numProcesses, timeCounter, CPUUseTime, IOUseTime);

}

int findProcessRR(ProcessData* processes[], int numProcesses, int* currentProcessIndex){
	int set = 0;
	int longestWait = -1;

	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == READY){
			if(processes[i]->currentWaitTime > longestWait){
				longestWait = processes[i]->currentWaitTime;
				*currentProcessIndex = i;
				set = 1;
			} else if(processes[i]->currentWaitTime == longestWait){
				if(processes[i]->A < processes[*currentProcessIndex]->A){
					longestWait = processes[i]->currentWaitTime;
					*currentProcessIndex = i;
				} 
			}
		}
	}

	return set;
}
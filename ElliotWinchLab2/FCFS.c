#include "FCFS.h"
#include "OutputBuilder.h"

#define MAX_ARRIVAL_TIME 1000000

void FCFS(ProcessData* processes[], int numProcesses, int verboseMode, FILE * randNumFile){

	//Init process counters
	int i;
	for (i = 0; i < numProcesses; i++){	
		if(processes[i]->A > MAX_ARRIVAL_TIME){
			printf("Process %d has an arrival time later than the maximum of %d. Please increase max arrival time to work with this test case.\n", i, MAX_ARRIVAL_TIME);
			exit(1);
		}
	}

	int timeCounter = -1;
	int CPUUseTime = 0;
	int IOUseTime = 0;

	int currentProcessIndex = -1;

	while(unFinishedProcesses(processes, numProcesses)){

		timeCounter++;

		if(verboseMode){
			printf("Before cycle %d: \t", timeCounter);
			verboseData(processes, numProcesses);
		}

		IOUseTime += checkBlockedProcesses(processes, numProcesses);
		checkArrivingProcesses(processes, numProcesses, timeCounter);

		//These might be combined for performance. They are separated for clarity.
		if(currentProcessIndex >= 0){

			runProcess(processes[currentProcessIndex]);
			CPUUseTime++;

			if(processes[currentProcessIndex]->totalCPUTimeRemaining <= 0){
				processes[currentProcessIndex]->state = TERMINATED;
				currentProcessIndex = -1;
			} else if(processes[currentProcessIndex]->CPUBurstTimeRemaining <= 0){
				processes[currentProcessIndex]->state = BLOCKED;
				processes[currentProcessIndex]->blockedTimeRemaining = processes[currentProcessIndex]->currentCPUBurstTime * processes[currentProcessIndex]->M;
				currentProcessIndex = -1;
			}
		}

		if(currentProcessIndex == -1){
			if(findProcess(processes, numProcesses, &currentProcessIndex)){
				processes[currentProcessIndex]->currentCPUBurstTime = randomOS(processes[currentProcessIndex]->B, randNumFile);
				processes[currentProcessIndex]->CPUBurstTimeRemaining = processes[currentProcessIndex]->currentCPUBurstTime;
				processes[currentProcessIndex]->state = RUNNING;
				processes[currentProcessIndex]->currentWaitTime = 0;
			}
		}

		incReadyProcesses(processes, numProcesses);
	}

	printf("\nThe scheduling algorithm used was First Come First Served.\n");

	processFinishedOutput(processes, numProcesses);

	summaryData(processes, numProcesses, timeCounter, CPUUseTime, IOUseTime);

}

int unFinishedProcesses(ProcessData* processes[], int numProcesses){
	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state != TERMINATED){
			return 1;
		}
	}

	return 0;
}

int checkBlockedProcesses(ProcessData* processes[], int numProcesses){
	int anyBlocked = 0;
	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == BLOCKED){
				processes[i]->IOTime++;
				processes[i]->blockedTimeRemaining--;
				anyBlocked = 1;

			if(processes[i]->blockedTimeRemaining <= 0){
				processes[i]->state = READY;
			} 
		}
	}

	return anyBlocked;
}

void checkArrivingProcesses(ProcessData* processes[], int numProcesses, int timeCounter){
	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == UNSTARTED){
			if(processes[i]->A <= timeCounter){
				processes[i]->state = READY;
			}
		}
	}
}

void runProcess(ProcessData* currentProcess){
	currentProcess->totalCPUTimeRemaining--;	 
	currentProcess->CPUBurstTimeRemaining--;
	currentProcess->CPUTime++;
}

void incReadyProcesses(ProcessData* processes[], int numProcesses){
	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == READY){
			processes[i]->waitTime++;
			processes[i]->currentWaitTime++;
		}
	}
}

int findProcess(ProcessData* processes[], int numProcesses, int* currentProcessIndex){
	int set = 0;
	int firstCome = -1;

	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == READY){
			if(processes[i]->currentWaitTime > firstCome){
				firstCome = processes[i]->currentWaitTime;
				*currentProcessIndex = i;
				set = 1;
			}
		}
	}

	return set;
}
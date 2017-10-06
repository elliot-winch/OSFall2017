#include "ProcessMethods.h"

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

int randomOS(int B, FILE* randNumFile){
	int randNum;
	fscanf(randNumFile, "%d", &randNum);

	printf("Rand num used: %d\n", randNum);

	return 1 + randNum%B;

}

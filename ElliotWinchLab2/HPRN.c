#include "FCFS.h"
#include "OutputBuilder.h"

void HPRN(ProcessData* processes[], int numProcesses, int verboseMode, FILE * randNumFile){

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
			if(findProcessHPRN(processes, numProcesses, &currentProcessIndex)){
				int burstTime = randomOS(processes[currentProcessIndex]->B, randNumFile);
				
				processes[currentProcessIndex]->currentCPUBurstTime = burstTime;
				processes[currentProcessIndex]->CPUBurstTimeRemaining = burstTime;

				processes[currentProcessIndex]->state = RUNNING;
				processes[currentProcessIndex]->currentWaitTime = 0;
			}
		}

		incReadyProcesses(processes, numProcesses);
	}

	printf("\nThe scheduling algorithm used was Highest Penality Ratio Next.\n");

	processFinishedOutput(processes, numProcesses);

	summaryData(processes, numProcesses, timeCounter, CPUUseTime, IOUseTime);

}

int findProcessHPRN(ProcessData* processes[], int numProcesses, int* currentProcessIndex){
	int set = 0;
	float HPR = 2;

	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == READY){

			float curHPR = 1;
			if(!(processes[i]->CPUTime + processes[i]->IOTime + processes[i]->waitTime) == 0){
				curHPR = processes[i]->CPUTime / (float)(processes[i]->CPUTime + processes[i]->IOTime + processes[i]->waitTime);
			} 
				
			if(curHPR < HPR){
				HPR = curHPR;
				*currentProcessIndex = i;
				set = 1;
			} else if(curHPR == HPR){
				if(processes[i]->A < processes[*currentProcessIndex]->A){
					HPR = curHPR;
					*currentProcessIndex = i;
				} 
			}
		}
	}

	return set;
}
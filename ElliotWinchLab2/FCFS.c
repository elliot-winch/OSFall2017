#include "FCFS.h"
#include "OutputBuilder.h"

void FCFS(ProcessData* processes[], int numProcesses, int verboseMode, FILE * randNumFile){

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
			if(findProcessFCFS(processes, numProcesses, &currentProcessIndex)){
				int burstTime = randomOS(processes[currentProcessIndex]->B, randNumFile);
				
				processes[currentProcessIndex]->currentCPUBurstTime = burstTime;
				processes[currentProcessIndex]->CPUBurstTimeRemaining = burstTime;

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

int findProcessFCFS(ProcessData* processes[], int numProcesses, int* currentProcessIndex){
	int set = 0;
	int firstCome = -1;

	int i;
	for(i = 0; i < numProcesses; i++){
		if(processes[i]->state == READY){
			if(processes[i]->currentWaitTime > firstCome){
				firstCome = processes[i]->currentWaitTime;
				*currentProcessIndex = i;
				set = 1;
			} else if(processes[i]->currentWaitTime == firstCome){
				//How come currentProcessIndex always points to the right
				//process? Because the above condition is always true if
				//there are ready processes, so currentProcessIndex
				//is set before this condition can possibly be met.
				//This is alway why we don't have to change the 'set' flag below.
				if(processes[i]->A < processes[*currentProcessIndex]->A){
					firstCome = processes[i]->currentWaitTime;
					*currentProcessIndex = i;
				} 
			}
		}
	}

	return set;
}
typedef struct ProcessData ProcessData;

int readProcesses(int numProcesses, ProcessData** processes, FILE* processFile);
void prepProcessData(ProcessData* src[], ProcessData* dest[], int numProcesses);
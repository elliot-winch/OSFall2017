#define MAX_DEFS 64

typedef struct Table
{
	int definitionCounter;
	char** names;
	int* vals;
} Table;

Table* initTable();
int getFromTable(Table* table, char* name);
int addToTable(Table* table, char* name, int val);
int nameEquals(char* n1, char* n2);
void modifyNameInTable(Table* table, char* name, int val);
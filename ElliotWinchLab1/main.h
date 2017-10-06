#include "table.h"

#define WORD_SIZE 5
#define MAX_ARG_NAME 16

typedef struct Module
{
	int numDefs;
	char** defNames;
	int* defVals;	//Relative address is saved

	int numUses;
	char** useNames;
	int* useVals;

	int numWords;
	char** words;
} Module;

Module** handleInput(Table* table, int* numMods);
void handleModule(Module* module, Table* table);
void relocateAddress(char* word, int amount);
int extractAddressFromWord(char* word);
void implantAddressIntoWord(char* word, int value);
void produceOutput(Table* table, Module** modules, int numModules);


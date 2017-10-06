#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.c"

int main(){

	printf("\n\n");

	Table* table = initTable();

	int numModules;
	Module** modules = handleInput(table, &numModules);

	int i;
	for(i = 0; i < numModules; i++){
		handleModule(modules[i], table);
	}

	produceOutput(table, modules, numModules);


	//Memory management
	for(i = 0; i < numModules; i++){
		Module * m = modules[i];

		int j;
		for(j = 0; j < m->numDefs; j++){
			free(m->defNames[j]);
		}
		free(m->defNames);
		free(m->defVals);

		for(j = 0; j < m->numUses; j++){
			free(m->useNames[j]);
		}
		free(m->useNames);
		free(m->useVals);

		for(j = 0; j < m->numWords; j++){
			free(m->words[j]);
		}
		free(m->words);

		free(m);
	}

	free(modules);

	for(i = 0; i < table->definitionCounter; i++){
		free(table->names[i]);
	}
	free(table->names);

	free(table->vals);

	free(table);

	return 0;
}

Module** handleInput(Table* table, int* numMods){

	int numModules;
	scanf("%i", &numModules);

	*numMods = numModules;

	if(numModules <= 0){
		printf("Error: The number of modules must be at least 1.\n");
		return;
	}

	int baseAddress = 0;

	Module** modules = malloc(numModules * sizeof(Module));

	int i;
	for(i = 0; i < numModules; i++){

		Module* m = malloc(sizeof(Module));
		modules[i] = m;

		//Set definitions
		int defNum;
		scanf("%i", &(defNum));

		m->numDefs = defNum;
		m->defNames = malloc(defNum * sizeof(char*));
		m->defVals = malloc(defNum * sizeof(int));

		int j;
		for(j = 0; j < defNum; j++){
			
			char* name = (char*) malloc(sizeof(char) * MAX_ARG_NAME);
			scanf("%s", name);
			int val;
			scanf("%i", &val);

			addToTable(table, name, val + baseAddress);

			m->defNames[j] = name;
			m->defVals[j] = val;
		}

		//Uses
		scanf("%i", &(m->numUses));

		m->useNames = malloc(m->numUses * sizeof(char*));

		int l;
		for(l = 0; l < m->numUses; l++){
			m->useNames[l] = malloc(MAX_ARG_NAME * sizeof(char));
		}

		m->useVals = malloc(m->numUses * sizeof(int));

		//Read uses
		for(j = 0; j < m->numUses; j++){
			scanf("%s", m->useNames[j]);
			scanf("%i", &(m->useVals[j]));
		}

		//Words
		scanf("%i", &(m->numWords));

		m->words = malloc(m->numWords * sizeof(char*));

		for(l = 0; l < m->numWords; l++){
			m->words[l] = malloc(WORD_SIZE * sizeof(char));
		}

		for(j = 0; j < m->numWords; j++){
			scanf("%s", m->words[j]);

			if(m->words[j][4] == '3'){
				relocateAddress(m->words[j], baseAddress);
			}
		}

		//Check to see if any of the defined values for this module were out of range
		int h;
		for(h = 0; h < defNum; h++){
			if(m->defVals[h] >= m->numWords){
				printf("The definition of %s is outside module %d. Assigned value 0.\n", m->defNames[h], i);
				m->defVals[h] = 0;
				modifyNameInTable(table, m->defNames[h], baseAddress);
			}
		}

		baseAddress += m->numWords;
	}

	return modules;
}


void relocateAddress(char* word, int amount){

	implantAddressIntoWord(word, extractAddressFromWord(word) + amount);
	
}

void handleModule(Module* module, Table* table){

	int i;
	for(i = 0; i < module->numUses; i++){
		int varVal = getFromTable(table, module->useNames[i]);

		if(varVal < 0){
			printf("Error: Attempted to use value which is not declared. %s has been given value 0.", module->useNames[i]);
			varVal = 0;
		}

		char* currentWord = module->words[module->useVals[i]];
		int currentAddress;

		do {
			if(currentWord[4] == '1'){
				printf("Word %s has address type internal but used in list. Treated as external.\n", currentWord);
			}

			currentAddress = extractAddressFromWord(currentWord);
			implantAddressIntoWord(currentWord, varVal);
			currentWord = module->words[currentAddress];
		} while(currentAddress != 777);
	}
}

int extractAddressFromWord(char* word){

	if(word[4] == '1'){
		printf("%s has address type immediate. Changing to external.\n", word);
		word[4] == '4';
	}

	int addressVal = 0;
	int converter = 1;
	int i;

	for(i = 3; i >= 1; i--){
		addressVal += (int)((word[i] - 48) * converter);
		converter = converter*10;
	}

	return addressVal;
}


void implantAddressIntoWord(char* word, int value){

	int i;
	for(i = 3; i >= 1; i--){
		word[i] = (char)((value%10) + 48);
		value = value/10;
	}
}

void produceOutput(Table* table, Module** modules, int numModules){

	printf("\nSymbol Table:\n");

	int i;
	for(i = 0; i < table->definitionCounter; i++){
		printf("%s, %i\n", table->names[i], table->vals[i]);
	}

	printf("\nMemory Map:\n");

	int j, k, counter;
	counter = 0;
	for(i = 0; i < numModules; i++){
		Module * currentModule = modules[i];
		for(j = 0; j < currentModule->numWords; j++){
			printf("%d: ", counter);
			for(k = 0; k < 4; k++){
				printf("%c", currentModule->words[j][k]);
			}
			printf("\n");
			counter++;
		}
	}

	printf("\n\n\n");
}
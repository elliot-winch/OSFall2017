#include <stdlib.h>
#include "main.h"

Table* initTable(){
	Table* table = (Table*) malloc(sizeof(Table));
	table->vals = malloc(sizeof(int) * MAX_DEFS);
	table->names = malloc(sizeof(char*) * MAX_DEFS);
	return table;
}

int nameEquals(char* n1, char* n2){

	int nameLength = strlen(n1);
	if(nameLength != strlen(n2)){
		return 0;
	}

	int i;
	for(i = 0; i < nameLength; i++){
		if(n1[i] != n2[i]){
			return 0;
		}
	}

	return 1;
}

int getFromTable(Table* table,char* name){

	int i;
	for(i = 0; i < table->definitionCounter; i++){
		if(nameEquals(name, table->names[i])){
			return table->vals[i];
		}
	}

	return -1;
}

void modifyNameInTable(Table* table, char* name, int val){

	int i;
	for(i = 0; i < table->definitionCounter; i++){
		if(nameEquals(name, table->names[i])){
			table->vals[i] = val;
		}
	}
}

int addToTable(Table* table, char* name, int val){

	int counter = table->definitionCounter;

	if(counter >= MAX_DEFS){
		printf("Definition limit reached!");
		return 0;
	}

	if(getFromTable(table, name) >= 0){
		printf("Value of %s already defined. Using first definition.\n", name);
		return 0;
	}

	table->names[counter] = (char*) malloc(sizeof(char) * strlen(name));
	table->names[counter] = name;
	table->vals[counter] = val;
	
	table->definitionCounter++;

	return 1;
}
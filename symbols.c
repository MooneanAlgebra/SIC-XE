#include "headers.h"

//Hash Function
unsigned int hash(char *string){

	int value = 0;
	for(int i=0; string[i]; i++)
			value+= string[i];

	return value%SIZE;
}

//initializa SymbolHashTable
void initSymTab(Symbol *SymTab[]){
	for(int i=0; i<SIZE; i++)
		SymTab[i]= (SYMBOL *)malloc(sizeof(SYMBOL));
}

//insert symbol into Symbol Table
int InsertSymbol( Symbol *SymTab[], char *name, int *addr){
	SYMBOL *x = malloc(sizeof(SYMBOL));
	int index = hash(name);
	while(SymTab[index] != NULL){
		index++;
	}
	x->SymTabIndex = index;
	strcpy(x->name, name);
	x->address = *addr;
	SymTab[index] = x;
	return index;
}

//search Symbol Table for string, return index if found, else return -1
int SearchSymTab(Symbol *SymTab[], char *testName){

	int length = strlen(testName)+1;
	char testNameNoBlanks[length];
	int x = 0;
	int i = 0;
	while(testName[i] != '\0'){
		if( testName[i] == ' ' || testName[i] == '\t')
			i++;
		else{
			testNameNoBlanks[x] = testName[i];
			i++;
			x++;
		}
	}
	testNameNoBlanks[x] = '\0';
	int index = hash(testNameNoBlanks);
	for(int i=index; SymTab[i] != NULL; i++){
		if(strcmp(testNameNoBlanks, SymTab[i]->name) == 0)
			return i;
	}

	return -1;

}

//print symbol table
void PrintSymTab(Symbol *SymTab[]){

	for(int i=0; i<SIZE; i++){
		if(SymTab[i])
			printf("|%s|\t|%X| \n", SymTab[i]->name, SymTab[i]->address);
	}
}

void freeSymTab(Symbol *SymTab[]){
	for(int i=0; i<SIZE; i++){
		if(SymTab[i]){
			free(SymTab[i]);
		}
	}
	free(SymTab);
}
//Check if string is a valid Symbol
int IsAValidSymbol(Symbol *SymTab[], char *TestSymbol ){

	int Result = 1;
	int maxlength = 6;
	char invalidChars[] = "$!=+-()@";
	int length = strlen(TestSymbol);

	if( IsADirective(TestSymbol) != 0){
		printf("SYMBOL IS DIRECTIVE. \n");
		Result = 0;
	}
	else if(SearchSymTab(SymTab, TestSymbol) != -1){
		printf("SYMBOL IS A DUPLICATE. \n");
		Result = 0;
	}
	else if(length > maxlength){
		printf("SYMBOL IS TOO LONG. \n");
       		Result = 0;
        }
        else if( !(TestSymbol[0] >= 65) && (TestSymbol[0] <= 90) ){
		printf("FIRST LETTER OF SYMBOL IS NOT A-Z. \n");
		Result = 0;
        }
	else if(strpbrk(TestSymbol, invalidChars) != NULL){
		printf("SYMBOL CONTAINS INVALID CHARACTER. \n");
		Result = 0;
	}
	else if(strpbrk(TestSymbol, " ") != NULL) {
		printf("SYMBOL IS EMPTY. \n");
		Result = 0;
	}
	else{
	Result = 1;
	}
	return Result;
}

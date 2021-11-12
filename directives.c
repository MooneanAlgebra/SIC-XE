#include "headers.h"

//check if string is a Directive and return the number associated  with that directive
int IsADirective( char *Test ){

	if(strcmp( Test, "START" ) == 0)
		return 1;
	else if(strcmp( Test, "BYTE" ) == 0)
		return 2;
	else if(strcmp( Test, "WORD" ) == 0)
		return 3;
	else if(strcmp( Test, "RESB" ) == 0)
		return 4;
        else if(strcmp( Test, "RESW" ) == 0)
		return 5;
	else if(strcmp( Test, "RESR" ) == 0)
		return 6;
	else if(strcmp( Test, "EXPORTS" ) == 0)
		return 7;
	else if(strcmp( Test, "END" ) == 0)
		return 8;
	else
		return 0;
}

//check if starting address is valid
int Start(Symbol *SymTab[], int index, char *c, int* address){
	int count = (int)strtol(c, NULL, 16);
	if(count >= 32768)
		return 0;
	SymTab[index]->address = count;
	*address = count;
	return 1;
}

//Check if third token is a char or a hex, remove indicator, update counter (named address)
int Byte(char *c, int *address){
	char *charOrHex = strtok(c, "'");
	if(strcmp(charOrHex, "C") == 0){
		char *constant = strtok(NULL, "'");
		*address += strlen(constant);
	}
	else if(strcmp(charOrHex, "X") == 0){
		char *constant = strtok(NULL, "'");
		if( ! (validHex(constant) ) )
			return 0;
		int length = strlen(constant);
		if((length % 2) == 1)
			return 0;
		*address += (length/2);
	}
	return 1;
}

void Word(int *address){
	*address += 3;
}
//update counter equal to the third token
void ResB(char *c, int *address){
	int x = atoi(c);
	*address += x;
}
//update counter equal to the third token * 3
void ResW(char *c, int *address){
	int x = atoi(c);
	*address += (x*3);
}

void ResR(int *address){
	*address += 3;
}

void Exports(int *address){
	*address += 3;
}

void End(int *address){
}


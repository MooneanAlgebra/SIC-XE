#include "headers.h"

void insertTRecord(TextRecord *TextRecords[], int address, int length, char *objCode){

	int index = 0;
	char objString[60]={0};
	int objCodePos = 0;
	int objLength = 0;
	int objParts = 0;

	TRecord *x = malloc(sizeof(TRecord));

	//Find empty position
	while(TextRecords[index] != NULL)
		index++;
	objLength = (int)strlen(objCode);

	//check if objCode is longer than one T record
	if(objLength > 60){
		objParts = objLength/59; //calculate how many T records need to be inserted

		for(int i=0; i<=objParts; i++){

			//copy first 59 characters (or until reaching null terminator) to objString
			for(int j=0; j<60; j++, objCodePos++){ 
				if(objCode[objCodePos] == '\0'){
					objString[j] = '\0';
					break;
				}
				objString[j] = objCode[objCodePos];
			}

			//insert T Record with substring
			insertTRecord(TextRecords, address, strlen(objString), objString);

			address += 30; //update address
			memset(objString, 0, sizeof(objString)); //reset substring
		}

	}
	//insert T Record
	else{

	x->startAddress = address;
	x->length = strlen(objCode);
	strcpy( x->objectCode, objCode);
	TextRecords[index] = x;
	}
}
//inserts a M Record into ModRecords
void insertMRecord(ModificationRecord *ModRecords[], int address, int modLength, char *name){

	int index = 0;
	MRecord *x = malloc(sizeof(MRecord));

	//find empty position
	while(ModRecords[index] != NULL)
		index++;
	x->startAddress = ++address;
	x->length = modLength;
	strcpy(x->symbolName, name);
	ModRecords[index] = x;
}

//check if third token is a string or a hex string, return 0 if Byte operand is invalid
int Byte2(char *c, char *hexString){

	char *charOrHex = strtok(c, "'");

	//if token is a string, convert to a hex string and return length
	if(strcmp(charOrHex, "C") == 0){
		int i = 0;
		int j = 0;
		char string[1024] = {0};
		strcpy(string, strtok(NULL, "'"));
		char asciiHexString[2*(strlen(string)+1)];
		while(string[i] != '\0'){
			sprintf((char*)(asciiHexString+j), "%02X", (int)string[i]);
			i++;
			j+=2;
		}
		asciiHexString[j] = '\0';
		strcpy(hexString, asciiHexString);
		return strlen(hexString);
	}
	else if(strcmp(charOrHex, "X") == 0){
		strcpy(hexString, strtok(NULL, "'"));
		int length = strlen(hexString);
		return (length/2);
	}
	return 0;

}

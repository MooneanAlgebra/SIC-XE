#include "headers.h"


int main( int argc, char* argv[]){

	//Main variable declaration

	FILE *fp;
	FILE *objFP;
	char line[SIZE];
	char *newsym;
	char *nexttoken;
	char *operand;
	char *operandCPY;
	char asciiHexString[SIZE*2] = {0};
	char objCode[SIZE] = {0};
	char hexString[SIZE*2]={0};
	char programStartName[7];
	int counter = 0;
	int startCounter = 0;
	int programLength = 0;
	int objLength = 0;
	int endRecordAddress = 0;
	int opTableIndex = 0;
	int currentIndex = 0;
	int startFlag = 0;
	int endFlag = 0;



	struct Symbol **SymbolHashTable = malloc(SIZE * sizeof(SYMBOL *));
	struct TextRecord **TextRecords = malloc(SIZE * sizeof(TRecord *));

	struct ModificationRecord **ModRecords = malloc (SIZE * sizeof(MRecord *));

	struct HeaderRecord header;

	//Check to see if there are 2 arguments
	if ( argc != 2 ) {
	printf("ERROR: Usage: %s filename.\n", argv[0]);
	return 0;
	}

	//open the file in "read" only mode
	fp = fopen( argv[1], "r");

	//If file being opened brings back NULL, throw error
	if ( fp == NULL ) {
		printf("ERROR: %s could not be opened for reading.\n", argv[1] );
		exit(0);
	}

	initOpcodeTable(); // Build opcode table

	// Memory Allocation
	newsym = malloc(  SIZE * sizeof(char) );
	memset( newsym, '\0', SIZE * sizeof(char) );
	nexttoken = malloc(  SIZE * sizeof(char) );
	memset( nexttoken, '\0', SIZE * sizeof(char) );
	operand = malloc( SIZE * sizeof(char) );
	memset( operand, '\0', SIZE * sizeof(char) ); 
	operandCPY = malloc(SIZE * sizeof(char) );
	memset(operandCPY, '\0', SIZE * sizeof(char) );

        //First pass
        while(  fgets( line , SIZE , fp ) != NULL   ) {
		if( counter >= 32768){
			printf("ERROR. PROGRAM DOES NOT FIT SIC MEMORY.\n");
			exit(0);
		}
		currentIndex = 0;

                //If the line starts with #, continue
                if ( line[0] == 35){
                        continue;
                }

                //If(first character is between A-Z)
                if (  ((line[0] >= 65 ) && ( line[0] <= 90 )) || line != NULL  )  {
			//tokenize the line using tabs and newline being delimiters.
                        newsym = strtok( line, " \t\n");
                        nexttoken = strtok( NULL, " \t\n"  ); // Gets next token by submitting null
                        operand = strtok( NULL, "\t\n" );

                        // error-checking
                        if(newsym == NULL && nexttoken == NULL){
                                printf("ERROR: SYMBOL & OPCODE/DIRECTIVE ARE EMPTY.");
                                exit(0);
                        }

                        if ( IsAValidSymbol(SymbolHashTable, newsym) == 0 ) {

                                printf("ERROR. %s IS AN  INVALID SYMBOL.\n", newsym);
                                exit(0);
                        }

                        //Check to see if first token is an opcode
			if( (IsAnOpcode(newsym) != -1)){
				operand = nexttoken;
				nexttoken = newsym;
				newsym = NULL;
				counter += 3;
				continue;
			}


			currentIndex = InsertSymbol(SymbolHashTable, newsym, &counter); //inserts new symbol into Hash Table

                        switch(IsADirective(nexttoken)){
				case 1: ; //START DIRECTIVE
					if(validHex(operand) == 0){
						printf("ERROR. INVALID HEX NUMBER. \n");
						exit(0);
					}
					if(Start(SymbolHashTable, currentIndex, operand, &counter)== 0){
						printf("ERROR. INVALID START NUMBER. \n");
						exit(0);
					}
					startFlag++;
					startCounter = counter;
				break;
                                case 2: //BYTE DIRECTIVE
					if(Byte(operand, &counter) == 0){
						printf("ERROR. ODD NUMBER OF HEXES. \n");
						exit(0);
					}
				break;
                                case 3:	//WORD DIRECTIVE
                                	if(atoi(operand) >= 8388607){
                                		printf("ERROR. WORD CONSTANT IS TOO LARGE. \n");
                                		exit(0);
                                	}

					Word(&counter);
                                break;
                                case 4: //RESB DIRECTIVE
					ResB(operand, &counter);
                                break;
                                case 5: //RESW DIRECTIVE
					ResW(operand, &counter);
                                break;
                                case 6: //RESR DIRECTIVE
					ResR(&counter);
                                break;
                                case 7: //EXPORTS DIRECTIVE
					Exports(&counter);
                                break;
				case 8: //END DIRECTIVE
					End(&counter);
					endFlag++;
				break;
				default :
		                        counter += 3;
				break;
			}
                }
	}

	if(startFlag != 1 || endFlag != 1){
		printf("ERROR. NONE/MORE THAN ONE START OR END DIRECTIVE PRESENT.\n");
		exit(0);
	} 

	rewind(fp); //set file pointer to beginning of file
	programLength = counter - startCounter; //calculates length of the program
	counter = 0; //reset counter
	char *objFile = strcat(argv[1], ".obj"); //"filenamr.obj"
	objFP = fopen(objFile , "w");
	//PASS 2

        while(  fgets( line , SIZE , fp ) != NULL   ) {

                //If the line starts with #, continue
                if ( line[0] == 35){
                        continue;
                }

                //If(first character is between A-Z)
                if (  ((line[0] >= 65 ) && ( line[0] <= 90 )) || line != NULL  )  {
			//tokenize line
        		newsym = strtok( line, " \t\n"); //token 1
                	nexttoken = strtok( NULL, " \t\n"  ); //token 2
                	operand = strtok( NULL, "\t\n" ); //token 3

			currentIndex = SearchSymTab(SymbolHashTable, newsym); //Check if first token is a symbol
			if(currentIndex != -1){ //Symbol has been found
			}
			else if( strcmp(newsym, "RSUB") == 0){ //Check for RSUB opcode
				opTableIndex = IsAnOpcode(newsym); //find position in opcode table
				strcpy(objCode, OpcodeTable[opTableIndex].OpCode); //copy opcode hex
				strcat(objCode, "0000");
				insertTRecord(TextRecords, counter, OBJ_LENGTH, objCode);
				counter +=3;
				continue;
			}
			else{	//if first token is not a symbol or RSUB opcode, rearrange token variables for consistency
				operand = nexttoken;
				nexttoken = newsym;
				newsym = NULL;
			}

			switch(IsADirective(nexttoken)){
				case 1: ; //START DIRECTIVE
					strcpy(header.name, newsym); //HEADER PROGRAM = First token
					header.startAddress = SymbolHashTable[currentIndex]->address; //HEADER STARTING ADDRESS = newsym.address
					header.length = programLength;	//HEADER LENGTH OF PROGRAM = programLength (caclulated after Pass 1)
					strcpy(programStartName, SymbolHashTable[currentIndex]->name); //programStartName = newsym.name (for Modification Records)
					counter = SymbolHashTable[currentIndex]->address; //update counter with symbol's address
				continue;
				case 2: //BYTE DIRECTIVE
					counter = SymbolHashTable[currentIndex]->address; //update counter with current symbol's address
					objLength = Byte2(operand, hexString); // remove char or hex indicator and return the length of the hex string
					insertTRecord(TextRecords, counter, objLength, hexString); //Insert into Text Records
				continue;
				case 3: //WORD DIRECTIVE
					counter = SymbolHashTable[currentIndex]->address; //update counter
					int x = atoi(operand); //convert operand into an int
					sprintf(asciiHexString,"%06X",x); //convert int operand to a hex string
					insertTRecord(TextRecords, counter, WORD_LENGTH, asciiHexString); //Insert into Text Records
				continue;
				case 4: //RESB DIRECTIVE
					counter = SymbolHashTable[currentIndex]->address; //update counter
				continue;
				case 5: //RESW DIRECTIVE
					counter = SymbolHashTable[currentIndex]->address; //update counter
				continue;
				case 6: //RESR DIRECTIVE
					counter = SymbolHashTable[currentIndex]->address; //update counter
				continue;
				case 7: //EXPORTS DIRECTIVE
					counter = SymbolHashTable[currentIndex]->address; //update counter
				continue;
				case 8: //END DIRECTIVE
					if(SearchSymTab(SymbolHashTable, operand) == -1){ //if symbol is not in SymbolHashTable, exit program
						printf("ERROR. SYMBOL IS NOT PRESENT IN SYMBOL TABLE.\n");
						exit(0);
					}
					endRecordAddress = SymbolHashTable[SearchSymTab(SymbolHashTable, operand)]->address; //endRecordAddress = Third token.address
					counter = SymbolHashTable[currentIndex]->address; //update counter
				continue;
				default :
				break;
			}
			opTableIndex = IsAnOpcode(nexttoken); //Find opcode within OpCodeTable
			strcpy(objCode, OpcodeTable[opTableIndex].OpCode); //copy opcode hex
			strcpy(operandCPY, operand); //copy third token to avoid unintentional modification of original

			//check for indirect addressing
			if(strstr(operandCPY, ",X") != NULL){
				char *t = strtok(operand, ",X");
				if(SearchSymTab(SymbolHashTable, t) == -1){
					printf("ERROR. SYMBOL %s IS NOT PRESENT IN SYMBOL TABLE.\n", t);
					exit(0);
				}
				int indirectAddress = SymbolHashTable[SearchSymTab(SymbolHashTable, t)]->address + 32768; //32768 = 8000 in hex
				sprintf(hexString, "%X", indirectAddress);
			}
			else{
				if(SearchSymTab(SymbolHashTable, operand) == -1){
					printf("ERROR. SYMBOL %s IS NOT PRESENT IN SYMBOL TABLE.\n", operand);
					exit(0);
				}
				sprintf(hexString,"%X", SymbolHashTable[SearchSymTab(SymbolHashTable, operand)]->address); //find referenced symbol, pull the address, convert address to hex, output to "hexString" variable
			}
			strcat(objCode,hexString); // add hexstring result to end of objCode

			if(newsym == NULL){ //If no symbols are present
				counter += 3;
				insertTRecord(TextRecords, counter, OBJ_LENGTH, objCode);
				insertMRecord(ModRecords, counter, strlen(hexString), programStartName);
			}
			else{
				counter = SymbolHashTable[currentIndex]->address;
				insertTRecord(TextRecords, counter, OBJ_LENGTH, objCode);
				insertMRecord(ModRecords, counter, strlen(hexString), programStartName);

			}
		}
	}

	//Write records to filename.obj
	fprintf(objFP, "H%s\t%06X%06X\n", header.name, header.startAddress, header.length);
	for(int i=0; i<SIZE; i++){
		if(TextRecords[i])
			fprintf(objFP, "T%06X%02X%s\n", TextRecords[i]->startAddress, TextRecords[i]->length, TextRecords[i]->objectCode);
	}

	for(int i=0; i<SIZE; i++){
		if(ModRecords[i])
			fprintf(objFP, "M%06X%02X+%s\n", ModRecords[i]->startAddress, ModRecords[i]->length, ModRecords[i]->symbolName);
	}
	fprintf(objFP, "E%06X\n", endRecordAddress);

	//Closing statements
	fclose( fp );
	fclose( objFP );
	free(SymbolHashTable);
	free(TextRecords);
	free(ModRecords);

	return 0;
}

int validHex(char *test){ //check if string is a valid hex string
	int length = strlen(test);
	for(int i=0; i<length; i++){
		if(! (isxdigit(test[i]) ) )
			return 0;
	}
	return 1;
}

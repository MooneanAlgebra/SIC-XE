#include "headers.h"

char *mnemonicArray[] = {
"ADD",
"ADDF",
"ADDR",
"AND",
"CLEAR",
"COMP",
"COMPF",
"COMPR",
"DIV",
"DIVF",
"DIVR",
"FIX",
"FLOAT",
"HIO",
"J",
"JEQ",
"JGT",
"JLT",
"JSUB",
"LDA",
"LDB",
"LDCH",
"LDF",
"LDL",
"LDS",
"LDT",
"LDX",
"LPS",
"MUL",
"MULF",
"MULR",
"NORM",
"OR",
"RD",
"RMO",
"RSUB",
"SHIFTL",
"SHIFTR",
"SIO",
"SSK",
"STA",
"STB",
"STCH",
"STF",
"STI",
"STL",
"STS",
"STSW",
"STT",
"STX",
"SUB",
"SUBF",
"SUBR",
"SVC",
"TD",
"TIO",
"TIX",
"TIXR",
"WD"
};

char *opcodeArray[] = {

"18",
"58",
"90",
"40",
"B4",
"28",
"88",
"A0",
"24",
"64",
"9C",
"C4",
"C0",
"F4",
"3C",
"30",
"34",
"38",
"48",
"00",
"68",
"50",
"70",
"08",
"6C",
"74",
"04",
"D0",
"20",
"60",
"98",
"C8",
"44",
"D8",
"AC",
"4C",
"A4",
"A8",
"F0",
"EC",
"0C",
"78",
"54",
"80",
"D4",
"14",
"7C",
"E8",
"84",
"10",
"1C",
"5C",
"94",
"B0",
"E0",
"F8",
"2C",
"B8",
"DC"
};

//build opcode table. Arrays positions are correctly placed beforehand
void initOpcodeTable(){
	int length = sizeof(mnemonicArray)/sizeof(mnemonicArray[0]);
	for(int i=0; i<length; i++)
		strncpy(OpcodeTable[i].OpCode, opcodeArray[i], 3);
	for(int i=0; i<length; i++)
		strncpy(OpcodeTable[i].Name, mnemonicArray[i], 8);

}

//print opcode table
void printOpcodeTable(){
	int length = sizeof(OpcodeTable)/sizeof(OpcodeTable[0]);
	for(int i=0; i<length; i++)
		printf("%s ---- %s \n", OpcodeTable[i].Name, OpcodeTable[i].OpCode);
}

//return index of opcode table if string is found, return -1 if not
int IsAnOpcode(char *test){
	int length = sizeof(OpcodeTable)/sizeof(OpcodeTable[0]);
	for(int i=0; i<length; i++){
		if(strcmp(test, OpcodeTable[i].Name) == 0)
			return i;

	}
	return -1;
}

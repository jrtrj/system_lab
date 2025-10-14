/* pass1.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100

struct optab{
	char opcode[MAX];
	char hex_code[MAX];
};

struct symtab{
	char symbol[MAX];
	int address;
};

//const == final
int search_symtab(struct symtab* mysymtab, int symtab_length, const char* label){
    for(int i = 0; i < symtab_length; i++) {
        if(strcmp(mysymtab[i].symbol, label) == 0) {
            return 1;
        }
    }
    return 0;
}

int search_optab(struct optab* myoptab, int optab_length, const char* opcode){
    for(int i = 0; i < optab_length; i++) {
        if(strcmp(myoptab[i].opcode, opcode) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
	struct optab myoptab[MAX];
	struct symtab mysymtab[MAX];
	FILE *fp_input, *fp_optab, *fp_symtab, *fp_inter;

    	int symtab_length = 0;
    	int optab_length = 0;
	int start_address = 0;
	int lc = 0; //location counter
	char label[MAX],opcode[MAX],operand[MAX];

	// Read the values in optab to memory
    fp_optab = fopen("optab.txt","r");
	while(fscanf(fp_optab,
				"%s %s",
				myoptab[optab_length].opcode,
				myoptab[optab_length].hex_code) != EOF)
	{
		optab_length++;		
	}
	fclose(fp_optab);

	fp_input = fopen("input.txt","r");
	fp_inter = fopen("inter.txt","w");

	//read the first input line
	fscanf(fp_input,"%s %s %s",label,opcode,operand);
	if(strcmp("START",opcode) == 0) {
		start_address = (int) strtol(operand,NULL,16);
		lc = start_address; // LOCCTR to starting address
		fprintf(fp_inter,"\t%s\t%s\t%s\n",label,opcode,operand); // write to intermediate
        // read next input line
		fscanf(fp_input,"%s %s %s",label,opcode,operand);	
	} else {
		lc = 0; //init LOCCTR to 0
	}
	
	while(strcmp(opcode,"END") != 0) {
        fprintf(fp_inter,"%X\t%s\t%s\t%s\n",lc,label,opcode,operand);

        if(strcmp(label,"-") != 0) {
            //duplicate SYMTAB entry
            if(search_symtab(mysymtab,symtab_length,label) == 1) {
                printf("Duplicate Label in SYMTAB: %s\n",label);
            }
            //copy the values to SYMTAB
            else{
                strcpy(mysymtab[symtab_length].symbol,label);
                mysymtab[symtab_length].address = lc;
                symtab_length++;
            }
        }
        //update LOCCTR based on opcode
        if(search_optab(myoptab,optab_length,opcode) == 1)
            lc += 3;
        else if(strcmp(opcode,"WORD") == 0)
            lc += 3;
        //3 * size of operand
        else if(strcmp(opcode,"RESW") == 0)
            lc += 3*atoi(operand);
        // Character or Hexadecimal
        else if(strcmp(opcode,"BYTE") == 0) {
            if(operand[0] == 'C'){
                //excluding C and ''
                lc += strlen(operand) - 3;
            } 
            // Each hex digit pair is 1 byte
            else if (operand[0] == 'X') {
               // two characters form a hex number (1 byte)
               lc += (strlen(operand) - 3 + 1) / 2;
            }
        }
        else if(strcmp(opcode,"RESB") == 0) {
            lc += atoi(operand);
        }
        else {
            printf("Invalid Opcode: %s\n",opcode);
        }

        // Read the next line from the input file
        fscanf(fp_input, "%s %s %s",label,opcode,operand);
	}
    
    // Print END line after the loop
    fprintf(fp_inter,"\t%s\t%s\t%s\n",label,opcode,operand);

    // Close files after they are no longer needed
    fclose(fp_input);
    fclose(fp_inter);

    // Write the SYMTAB to the File
    fp_symtab = fopen("symtab.txt","w");
    for(int i = 0; i < symtab_length; i++) {
        fprintf(fp_symtab,"%s\t%X\n",mysymtab[i].symbol,mysymtab[i].address);
    }
    fclose(fp_symtab);

    printf("Program length: %X\n", lc - start_address);

    return 0;
}

/* pass2.c */
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


const char* get_opcode_hex(struct optab* myoptab, int optab_length, const char* opcode){
    for(int i = 0; i < optab_length; i++) {
        if(strcmp(myoptab[i].opcode, opcode) == 0) {
            return myoptab[i].hex_code;
        }
    }
    return "not_found";
}

int get_symbol_address(struct symtab* mysymtab, int symtab_length, const char* symbol){
    for(int i = 0; i < symtab_length; i++) {
        if(strcmp(mysymtab[i].symbol, symbol) == 0) {
            return mysymtab[i].address;
        }
    }
    return -1;
}


int main() {
	struct optab myoptab[MAX];
	struct symtab mysymtab[MAX];
	FILE *fp_inter, *fp_optab, *fp_symtab, *fp_listing, *fp_obj;

    int symtab_length = 0;
    int optab_length = 0;

	int address, start_address, program_length;
	char label[MAX], opcode[MAX], operand[MAX];

    char text_record[MAX * 2] = "";
    int text_record_len = 0;
    int record_start_addr = 0;


    // --- 1. Load OPTAB and SYMTAB into Memory ---

    // Read OPTAB from file into the myoptab array
    fp_optab = fopen("optab.txt","r");
	while(fscanf(fp_optab, "%s %s", myoptab[optab_length].opcode, myoptab[optab_length].hex_code) != EOF) {
		optab_length++;
	}
	fclose(fp_optab);

    // Read SYMTAB from file into the mysymtab array
    fp_symtab = fopen("symtab.txt","r");
    while(fscanf(fp_symtab, "%s %X", mysymtab[symtab_length].symbol, &mysymtab[symtab_length].address) != EOF) {
        symtab_length++;
    }
    fclose(fp_symtab);


    // --- 2. Main Processing Logic ---

	fp_inter = fopen("inter.txt","r");
	fp_listing = fopen("listing.txt","w");
    	fp_obj = fopen("object_code.txt","w");

	// Read the first line (START directive) from intermediate file
	fscanf(fp_inter, "%s\t%s\t%s", label, opcode, operand);
	if(strcmp("START", opcode) == 0) {
		start_address = (int) strtol(operand, NULL, 16); // Convert hex operand to int
        record_start_addr = start_address;
		fprintf(fp_listing,"\t%s\t%s\t%s\n", label, opcode, operand);
	}

    // --- 3. Generate Header Record ---
    // The program length is now given, so we write the header record immediately.
    printf("Enter the program length (in hex): ");
    scanf("%X", &program_length);
    fprintf(fp_obj, "H^%s^%06X^%06X\n", label, start_address, program_length);


    // Main loop: Read intermediate file until the "END" directive
	while(fscanf(fp_inter, "%X\t%s\t%s\t%s", &address, label, opcode, operand) != EOF && strcmp(opcode, "END") != 0) {
        char object_code[MAX] = "";
        char current_code[MAX] = "";

        // Case 1: Opcode is found in OPTAB (standard instruction)
        const char* opcode_hex = get_opcode_hex(myoptab, optab_length, opcode);
        if(strcmp(opcode_hex, "not_found") != 0) {
            int operand_addr = 0;
            if(strcmp(operand, "-") != 0) {
                operand_addr = get_symbol_address(mysymtab, symtab_length, operand);
                if(operand_addr == -1) {
                    printf("Error: Symbol not found in SYMTAB: %s\n", operand);
                    operand_addr = 0; // Default to 0000 on error
                }
            }
            sprintf(object_code, "%s%04X", opcode_hex, operand_addr);
        }
        // Case 2: WORD directive
        else if(strcmp(opcode, "WORD") == 0) {
            sprintf(object_code, "%06X", atoi(operand));
        }
        // Case 3: BYTE directive
        else if(strcmp(opcode, "BYTE") == 0) {
            if(operand[0] == 'C') { // Character constant
                for(int i = 2; i < strlen(operand) - 1; i++) {
                    sprintf(current_code, "%X", operand[i]); // Convert char to its ASCII hex value
                    strcat(object_code, current_code);
                }
            } else if (operand[0] == 'X') { // Hexadecimal constant
                for(int i = 2; i < strlen(operand) - 1; i++) {
                    sprintf(current_code, "%c", operand[i]);
                    strcat(object_code, current_code);
                }
            }
        }
        // Case 4: RESW or RESB (no object code generated)
        else {
            strcpy(object_code, "");
        }

        // --- 4. Text Record Management ---

        // If object code was generated for this line...
        if (strlen(object_code) > 0) {
            // Check if the new object code will fit in the current text record
            // Max length is 30 bytes, which is 60 hex characters.
            if (text_record_len + strlen(object_code) > 60) {
                // Write the current text record to the object file
                fprintf(fp_obj, "T^%06X^%02X%s\n", record_start_addr, text_record_len / 2, text_record);
                // Start a new text record
                strcpy(text_record, object_code);
                text_record_len = strlen(object_code);
                record_start_addr = address;
            } else {
                // Append object code to the current text record
                strcat(text_record, "^");
                strcat(text_record, object_code);
                text_record_len += (1 + strlen(object_code)); // +1 for the '^' separator
            }
        }
        // If we hit RESW/RESB, it forces a new text record
        else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            if (text_record_len > 0) {
                fprintf(fp_obj, "T^%06X^%02X%s\n", record_start_addr, (text_record_len- ( (text_record_len+1)/3 -1) )/2 , text_record); // Adjust length for separators
            }
            strcpy(text_record, "");
            text_record_len = 0;
            // The start address for the *next* record will be set when we find the next instruction
            record_start_addr = 0;
        }

        if (record_start_addr == 0 && strlen(object_code) > 0) {
            record_start_addr = address;
        }


        // Write the line to the listing file
        fprintf(fp_listing, "%X\t%s\t%s\t%s\t%s\n", address, label, opcode, operand, object_code);
	}

    // --- 5. Write the final Text and End Records ---

    // Write the last text record (if any)
    if(text_record_len > 0) {
        fprintf(fp_obj, "T^%06X^%02X%s\n", record_start_addr, (text_record_len- ( (text_record_len+1)/3 -1) )/2, text_record);
    }

    // Write the END record
    fprintf(fp_obj, "E^%06X\n", start_address);

    // Write the final line to the listing file
    fprintf(fp_listing, "\t%s\t%s\t%s\n", label, opcode, operand);

    // The header record is now written at the beginning, so we no longer calculate length here.


    // --- 6. Cleanup ---
    fclose(fp_inter);
    fclose(fp_listing);
    fclose(fp_obj);

    printf("Pass 2 complete. Check listing.txt and object_code.txt\n");

    return 0;
}


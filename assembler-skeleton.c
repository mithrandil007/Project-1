/* Assembler code fragment for RiSC-16 assembler */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINELENGTH 1000

char * readAndParse(FILE *, char *, char **, char **, char **,
	char **, char **);
int isNumber(char *);

int find(char *s);

short complement(short x);

struct la{
	char lab[20];
	int address;
	int contents;
};

struct la obj[100];


int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char *label, *opcode, *arg0, *arg1, *arg2;
	char lineString[MAXLINELENGTH+1];


	char * content_copy;

	

	int i = 0;
	while(i<100){
		strcpy(obj[i].lab,"\0");
		i++;
	}
	//obj[99].address = 1000;
	// printf("obj[99]: %d\n",obj[99].address);
	int address_count = 0;


	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* here is an example for how to use readAndParse
	to read a line from inFilePtr */
	while(1){
	if (readAndParse(inFilePtr, lineString, &label, &opcode, &arg0, &arg1, &arg2) == NULL) {
		/* reached end of file */
	//	printf("eof?\n");
		break;
	} else {
		/* label is either NULL or it points to
		a null-terminated string in lineString
		that has the label. If label is NULL,
		that means the label field didn't exist.
		Same for opcode and argX. */
	//	printf("%s\n",opcode);
		if(label!=NULL){
	//	printf("label: %s     count: %d\n",label,address_count);
		//	printf("execute\n");
		 // obj[address_count].lab = label;
		  obj[address_count].address = address_count;
			strcpy(obj[address_count].lab,label);
			if(!strcmp(opcode,".fill")){
				//strcpy(obj[address_count].contents,arg0);
				strcpy(content_copy,arg0);

				obj[address_count].contents = atoi(content_copy);
			}
	}
	}
	address_count++;   //move to next address or instruction
}

int k;
for(k=0;k<20;k++){
	printf("%s\n",obj[k].lab);
}

printf("data456: %d\n",obj[19].contents);
//  i = 0;
// while(1){
// 	if(obj[i].lab != NULL){
// 		if(!strcmp(obj[i].lab,"data1")){
// 			printf("Match: %d\n",i);
// 			break;
// 		}
// 	}
// 	i++;
// }

//printf("complement %04hx\n",-1);

int result;


	/* this is how to rewind the file ptr so that you
	start reading from the beginning of the file */
	rewind(inFilePtr);

	address_count = 0;


	while(1){
		if (readAndParse(inFilePtr, lineString, &label, &opcode, &arg0, &arg1, &arg2) == NULL) {
		/* reached end of file */
	//	printf("eof?\n");
		break;
	}else{
		if(!strcmp(opcode,"add")){
			
			short op_add;
			short ar1_add,ar2_add,ar3_add;
			ar1_add = atoi(arg0);
			ar2_add = atoi(arg1);
			ar3_add = atoi(arg2);
			op_add = (0 << 13) | (ar1_add << 10) | (ar2_add << 7) | ar3_add;

			fprintf(outFilePtr,"%04hx\n",op_add);
			

		}
		else if(!strcmp(opcode,"addi")){
			short op_addi;
			short ar1_addi,ar2_addi,ar3_addi;
			int result_addi;
			short ar3_cont;
			ar1_addi = atoi(arg0);
			ar2_addi = atoi(arg1);
			isNumber(arg2)?(ar3_addi = atoi(arg2)):(result_addi = find(arg2),ar3_addi = obj[result_addi].address);
			
			ar3_cont = obj[ar3_addi].contents;

			printf("arg0: %d arg1: %d  ar3_add: %d",ar1_addi,ar2_addi,ar3_addi);

			op_addi = (1 << 13) | (ar1_addi << 10) | (ar2_addi << 7) | (ar3_cont);

			fprintf(outFilePtr,"%04hx\n",op_addi);

		
		}
		else if(!strcmp(opcode,"nand")){
			short op_nand;
			short ar1_nand,ar2_nand,ar3_nand;
			ar1_nand = atoi(arg0);
			ar2_nand = atoi(arg1);
			ar3_nand = atoi(arg2);
			op_nand = (2 << 13) | (ar1_nand << 10) | (ar2_nand << 7) | ar3_nand;

			fprintf(outFilePtr,"%04hx\n",op_nand);
		
			//printf("Match123123: %d\n",result);
		}
		else if(!strcmp(opcode,"lui")){
			int result_lui;
			short ar1_lui,ar2_lui;
			short op_lui;
			ar1_lui = atoi(arg0);
			if(isNumber(arg1)){
				//store directly in opcode
				ar2_lui = atoi(arg1);
			}
			else{
				//get address of label and store in opcode
				result_lui = find(arg1);
				ar2_lui = obj[result_lui].address;

			}
			if(ar2_lui < 0){
				ar2_lui = ar2_lui & 0x03ff;
			}
			op_lui = (3 << 13) | (ar1_lui << 10) | ar2_lui; 

			fprintf(outFilePtr,"%04hx\n",op_lui);

		}
		else if(!strcmp(opcode,"sw")){
			//result = find(:arg2);
			
			short op_sw;
			short ar1_sw,ar2_sw,ar3_sw;
			ar1_sw = atoi(arg0);
			ar2_sw = atoi(arg1);
			isNumber(arg2)?(ar3_sw = atoi(arg2)):(result = find(arg2),ar3_sw = obj[result].address);
			//ar3_sw = obj[result].address;

				if(ar3_sw < 0){
				ar3_sw = ar3_sw & 0x007f;
			}


			op_sw = (4 << 13) | (ar1_sw << 10) | (ar2_sw << 7) | ar3_sw;

			fprintf(outFilePtr,"%04hx\n",op_sw);
		}
		else if(!strcmp(opcode,"lw")){
			//result = find(arg2);
			//short op_lw = 5;

			short ar1_lw,ar2_lw,ar3_lw;
			ar1_lw = atoi(arg0);
			ar2_lw = atoi(arg1);
			isNumber(arg2)?(ar3_lw = atoi(arg2)):(result = find(arg2),ar3_lw = obj[result].address);
			//ar3_lw = obj[result].address;
			short op_final_lw;

			if(ar3_lw < 0){
				ar3_lw = ar3_lw & 0x007f;
			}

			//printf("data123: %d %d  %d\n",ar1,ar2,ar3);
			op_final_lw = (5 << 13) | (ar1_lw << 10) | (ar2_lw << 7) | ar3_lw;
		//	printf("after shift: %04hx\n",final);
			fprintf(outFilePtr,"%04hx\n",op_final_lw);
			//negative immediate value for lw
			
		
			//printf("Match123123: %d\n",result);
		}
		else if(!strcmp(opcode,"bne")){
			//result = find(arg2);

			printf("bne\n");
			short op_bne;
			short ar1_bne,ar2_bne,ar3_bne;
			int branch_addr_bne;
			ar1_bne = atoi(arg0);
			ar2_bne = atoi(arg1);
			if(isNumber(arg2)){
				ar3_bne = atoi(arg2);
			}
			else{
				result = find(arg2);
			branch_addr_bne = obj[result].address;
			ar3_bne = branch_addr_bne - (address_count+1);
			printf("ar3_bne: %hd\n",ar3_bne);
		}
			if(ar3_bne < 0){
				ar3_bne = ar3_bne & 0x007f;
			}

			op_bne = (6 << 13) | (ar1_bne << 10) | (ar2_bne << 7) | ar3_bne;

			//bne = 0xfff
			fprintf(outFilePtr,"%04hx\n",op_bne);
		
		}
		else if(!strcmp(opcode,"jalr")){
			short op_jalr;
			short ar1_jalr,ar2_jalr,ar3_jalr;
			
			ar1_jalr = atoi(arg0);
			ar2_jalr = atoi(arg1);
			op_jalr = (7 << 13) | (ar1_jalr << 10) | (ar2_jalr) << 7;
			fprintf(outFilePtr,"%04hx\n",op_jalr);
		}
		else if(!strcmp(opcode,"nop")){
	//		result = find(arg2);
			int op_nop = 0x0000;
			printf("nop\n");
			fprintf(outFilePtr,"%04x\n",op_nop);
		}
		else if(!strcmp(opcode,"halt")){
	//		result = find(arg2);
			int halt_op = 0xe071;
			fprintf(outFilePtr,"%04x\n",halt_op);
		
		printf("halt\n");
		}
		else if(!strcmp(opcode,".fill")){
	//		result = find(arg2);
			short res_fill;
			res_fill = atoi(arg0);
		
			fprintf(outFilePtr,"%04hx\n",res_fill);
			printf(".fill %s\n",arg0);
			//fprintf(outFilePtr,)
		}
	}
	address_count++;
	}

}

char * readAndParse(FILE *inFilePtr, char *lineString,
	char **labelPtr, char **opcodePtr, char **arg0Ptr,
	char **arg1Ptr, char **arg2Ptr)
{
	/* read and parse a line
	note that lineString must point to allocated memory,
		so that *labelPtr, *opcodePtr, and *argXPtr
		won't be pointing to readAndParse's memory
	also note that *labelPtr, *opcodePtr, and *argXPtr
		only point to memory in lineString.
	When lineString changes, so will *labelPtr,
		*opcodePtr, and *argXPtr
	returns NULL if at end-of-file */

	char *statusString, *firsttoken;
	statusString = fgets(lineString, MAXLINELENGTH, inFilePtr);
	if (statusString != NULL) {
		firsttoken = strtok(lineString, " \t\n");
		if (firsttoken == NULL || firsttoken[0] == '#') {
			return readAndParse(inFilePtr, lineString, labelPtr, opcodePtr, arg0Ptr, arg1Ptr, arg2Ptr);
		} else if (firsttoken[strlen(firsttoken) - 1] == ':') {
			*labelPtr = firsttoken;
			*opcodePtr = strtok(NULL, " \t\n");
			//printf("%c\n",**opcodePtr);
			firsttoken[strlen(firsttoken) - 1] = '\0';
		} else {
			*labelPtr = NULL;
			*opcodePtr = firsttoken;
		}
		*arg0Ptr = strtok(NULL, ", \t\n");
		*arg1Ptr = strtok(NULL, ", \t\n");
		*arg2Ptr = strtok(NULL, ", \t\n");
	}
	return(statusString);
}

int isNumber(char *string) {
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int find(char *s){
	int aj=0;
	while(1){
				if(obj[aj].lab != "\0"){
					if(!strcmp(obj[aj].lab,s)){
						break;
					}
				}
				aj++;
				
			}
			return aj;	
}

short complement(short x){
	return (~x + 1);
}


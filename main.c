// brainfvck-modified interpreter //
// copyright (C) 2016 isy //
// under the GPLv2-License (SEE LICENSE.md) //

#include <stdio.h>
#define ARRSIZE 30000 // TODO: make dynamic
 
//char input[] = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
char* input;
char* array;
int counter = 0;
 
int loopmode = 0;
int beginloop = 0;
 
int debugprintarray() {
	printf("\nDEBUG: [");
	for (int i = 0; i < ARRSIZE; ++i) {
		if (i == ARRSIZE-1)
			printf("\"%c\" = %d", array[i], array[i]);
		else
			printf("\"%c\" = %d, ", array[i], array[i]);
	}
	printf("]\n");
 
	return 0;
}
 
int interpret(char x, int i) {
	//printf("Interpreting '%c' at pos %d\n", x, i);
	switch (x) {
		case '>':
			++counter;
			break;
		case '<':
			--counter;
			break;
		case '+':
			++(array[counter]);
			//printf("Char at pos %d is now %d\n", counter, array[counter]);
			break;
		case '-':
			--(array[counter]);
			//printf("Char at pos %d is now %d\n", counter, array[counter]);
			break;
		case '.':
			printf("%c", array[counter]);
			break;
		case ',':
			array[counter] = getchar();
			break;
		case '[':
			loopmode = 1;
			beginloop = i;
			break;
		//case ']':
		//	loopmode = 0;
		//	break;
		case '#': // debugging command, prints out the tape
			debugprintarray();
			break;
		default:
			break;
	}
 
	return 0;
}
 
void runbrain(char* code, int size) {
	//printf("Code: %s, size: %d\n", code, size);
	printf("Output: ");
	for (int i = 0; i < size; ++i) {
		//printf("I: %d, char: %c\n", i, code[i]);
		interpret(code[i], i);
		if (loopmode) {
			if (code[i] == ']') {
				if (array[counter]) {
					i = beginloop;
					//printf("array counter: %d %d\n", array[counter], counter);
				}
				else
					loopmode = 0;
			}
		}
	}
	printf("\nProgram finished.\n");
}
 
int main(int argc, char** argv) {
	if (argc <= 1) {
		printf("brainfvck-modified interactive mode\n");
		while (1) {
			char* str = (char*)calloc(1, ARRSIZE);
			//memset(str, 0, ARRSIZE);
			printf("> ");
			gets(str);
			if (str[0] == '\0')
				goto end;
			array = (char*)calloc(1, ARRSIZE);
			runbrain(str, strlen(str));
			free(array);
			free(str);
		}
	}
 
	// let's read the file in argument 1
	FILE* myfile = fopen(argv[1], "rb");
 
	// obtain file size
	fseek(myfile, 0, SEEK_END);
	int filesize = ftell(myfile);
	rewind(myfile);
 
	// load the file into "input"
	input = (char*)malloc(filesize+1);
	fread(input, 1, filesize, myfile);
	input[filesize] = '\0';
	fclose(myfile);
	//printf("Now input contains: %s\n", input);
 
	array = (char*)calloc(1, ARRSIZE);
	//int size = sizeof(input)-1;
	int size = filesize;
 
	runbrain(input, filesize);
 
end:
	return 0;
}

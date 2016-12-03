// brainfvck-modified interpreter //
// copyright (C) 2016 isy //
// under the GPLv2-License (SEE LICENSE.md) //

#include <stdio.h>
#define ARRSIZE 30000 // TODO: make dynamic
 
//char input[] = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
char* input;
char* array;
int counter = 0;
int debug = 0;
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
	if (debug == 1) {
		printf("Interpreting '%c' at pos %d\n", x, i);
	}
	switch (x) {
		case '>':
			++counter;
			break;
		case '<':
			--counter;
			break;
		case '+':
			++(array[counter]);
			if (debug == 1) {
				printf("Char at pos %d is now %d\n", counter, array[counter]);
			}
			break;
		case '-':
			--(array[counter]);
			if (debug == 1) {
				printf("Char at pos %d is now %d\n", counter, array[counter]);
			}
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
                case '!':
                        printf("exiting...\n");
                        exit(0);
                        break;
		case '#': // debugging command, prints out the tape
			if (debug == 1) {
				debugprintarray();
				break;
			}
		case '@': // debugging command, frees array
			if (debug == 1) {
				free(array);
				break;
			}
		case '?': // help command
			printf ("\nbfm - an expanded brainfuck interpreter.\n\n> - move pointer right.\n< - move pointer left.\n+ - add 1 to current cell.\n- - subtract 1 from current cell.\n. - print contents of current cell in ASCII.\n, - ask for one char of user input.\n! - end program.\n# - print contents of tape.\n@ - clear tape.\n? - shows this help dialogue.\n");
		default:
			break;
	}
 
	return 0;
}
 
void runbrain(char* code, int size) {
	if (debug == 1) {
	printf("Code: %s, size: %d\n", code, size);
	}
	for (int i = 0; i < size; ++i) {
		if (debug == 1) {
			printf("I: %d, char: %c\n", i, code[i]);
		}
		interpret(code[i], i);
		if (loopmode) {
			if (code[i] == ']') {
				if (array[counter]) {
					i = beginloop;
					if (debug == 1) {
						printf("array counter: %d %d\n", array[counter], counter);
					}
				}
				else
					loopmode = 0;
			}
		}
	}
}
 
int main(int argc, char** argv) {
	if (argc <= 1) {
		printf("brainfvck-modified interactive shell\n");
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
	
	if (!strcmp(argv[2],"-d")) {
		printf("brainfvck-modified interactive shell. (debug mode)\n");
                while (1) {
		      debug = 1;
		char* str = (char*)calloc(1, ARRSIZE);
		printf("> ");
		gets(str);
		array = (char*)calloc(1, ARRSIZE);
		runbrain(str, strlen(str));
		// don't free array, use @ to manually free it.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CMD_LEN 512 /*max command line length*/
#define MAX_CMD_PIPES 10 /*max commands in a pipeline*/
#define MAX_CMD_ARGS 10 /*max args to a command*/

/********************* Structures *********************/

typedef struct cmdFile {
	char *name;
	int stage;
	char *args[MAX_CMD_ARGS]; /*NULL if not set*/
	int argc;
	int inStage; /*-1 if not set*/
	int outStage; /*-1 if not set*/
	int inFd; /*-1 if not set*/
	int outFd; /*-1 if not set*/
} cmdFile;

typedef struct fileSet {
	cmdFile files[MAX_CMD_PIPES];
	int size;
} fileSet;

typedef struct input {
	char *words[MAX_CMD_PIPES * MAX_CMD_ARGS + 1];
	int size;
} input;

/********************* Input *********************/

/*initializes the input string, setting spaces to null*/
/*adds pointers to the word array for each word*/
input *initInput(char *str);

/*checks for any errors in the input string*/
/*returns 0 if none, 1 if errors*/
int inputErrorCheck(input *in);

/********************* FileSet *********************/

/*initializes and returns a pointer to the fileset*/
/*initializes all cmdFiles within*/
fileSet *initFileSet();

/*makes the fileset based on the in string*/
fileSet *makeFileSet(input *in);

/********************* CmdFile *********************/

/*sets a cmdFile to given values*/
void setCmdFileBase(cmdFile *cf, char *name, int stage, 
				 	char **args, int argc);

/*sets a cmdFiles in and out pipe info*/
void setCmdFileIO(cmdFile *cf, int inStage, int outStage,
				  	int inFd, int outFd);

/********************* Funcs *********************/

/********************* Printing *********************/

/*prints the stages of the pipeline*/
void printPipeline(fileSet *fs);

/*prints one stage of the pipeline*/
void printStage(cmdFile *cf); 
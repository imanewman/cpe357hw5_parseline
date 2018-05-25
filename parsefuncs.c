#include "parseline.h"

/********************* Input *********************/

/*initializes the input string, setting spaces to null*/
/*adds pointers to the word array for each word*/
input *initInput(char *str) {
	input *in = (input *)malloc(sizeof(input));

	in->size = 0;

	/*set a pointer to each word, while setting all spaces and \n to null*/
	in->words[in->size++] = strtok(str, " \n");

	while ((in->words[in->size++] = strtok(NULL, " \n"))) {
		if (in->size == (MAX_CMD_PIPES * (MAX_CMD_ARGS + 3) + 1) 
			&& in->words[in->size - 1] != NULL) {
			/*perror("pipeline too deep or too many arguments\n");*/
			break;
		}
	}

	in->size--; /*decrement for last null pointer*/

	return in;
}

/*checks for any errors in the input string*/
/*returns 0 if none, 1 if errors*/
int inputErrorCheck(input *in) {
	int pipelen = 0;
	int cmdargs = 0;
	int inset = 0; /*set to 1 for pipe, 2 for redirect*/
	int outset = 0; /*set to 1 for pipe, 2 for redirect*/
	char *curCmd;
	int i;

	for (i = 0; i < in->size; i++) {
		if (cmdargs == 0) { /*if new cmd*/
			/*check if first arg is missing*/
			if (in->words[i][0] == '|' || in->words[i][0] == '<' || in->words[i][0] == '>') {
				perror("invalid null command\n");
				return 1;
			} else {
				curCmd = in->words[i];

				if (++pipelen > MAX_CMD_PIPES) { /*check if pipe too long*/
					perror("pipeline too deep\n");
					return 1;
				}
			}
		} else { /*if same cmd*/
			switch (in->words[i][0]) {
				case '|': 
					if (outset == 2) { /*if out already set*/
						fprintf(stderr, "%s: ambiguous output\n", curCmd);
						return 1;
					} else { /*reset cmdargs if new cmd coming*/
						cmdargs = 0;
						inset = 1;
						outset = 0;
					}
					break;
				case '<':
					if (inset == 1) { /*if in already set by pipe*/
						fprintf(stderr, "%s: ambiguous input\n", curCmd);
						return 1;
					} else if (inset == 2) { /*if in already set by redir*/
						fprintf(stderr, "%s: bad input redirection\n", curCmd);
						return 1;
					} else { /*if not already set*/
						inset = 2;
						if (in->words[i+1][0] == '|' || in->words[i+1][0] == '<' /* if name is missing*/
							|| in->words[i+1][0] == '>' || in->words[i+1][0] == '\0') {
							fprintf(stderr, "%s: bad input redirection\n", curCmd);
							return 1;
						}
					}
					break;
				case '>': 
					if (outset == 2) { /*if out already set by redir*/
						fprintf(stderr, "%s: bad output redirection\n", curCmd);
						return 1;
					} else { /*if not already set*/
						outset = 2;
						if (in->words[i+1][0] == '|' || in->words[i+1][0] == '<' /* if name is missing*/
							|| in->words[i+1][0] == '>' || in->words[i+1][0] == '\0') {
							fprintf(stderr, "%s: bad output redirection\n", curCmd);
							return 1;
						}
					}
					break;
				default:
					if (++cmdargs > MAX_CMD_ARGS) { /*check if args too long*/
						fprintf(stderr, "%s: too many arguments\n", curCmd);
						return 1;
					}
			}

		}
	}

	return 0;
}

/********************* FileSet *********************/

/*initializes and returns a pointer to the fileset*/
/*initializes all cmdFiles within*/
fileSet *initFileSet() {
	fileSet *fs = (fileSet *)malloc(sizeof(fileSet));
	int i;

	fs->size = 0;

	for (i = 0; i < MAX_CMD_PIPES; i++) {
		setCmdFileBase(fs->files + i, NULL, -1, NULL, 0);
		setCmdFileIO(fs->files + i, -1, -1, -1, -1);
	}

	return fs;
}

/*makes the fileset based on the in string*/
fileSet *makeFileSet(input *in) {
	fileSet *fs = initFileSet();
	int i;

	for (i = 0; i < in->size; i++) {
		/*TODO: fill in each files data */
		switch (in->words[i][0]) {
			case '|':
				break;
			case '>':
				break;
			case '<':
				break;
			default:
				break;
		}
	}

	return fs;
}

/********************* CmdFile *********************/

/*sets a cmdFile to given values*/
void setCmdFileBase(cmdFile *cf, char *name, int stage, 
				 	char **args, int argc) {
	int i;

	cf->name = name;
	cf->stage = stage;
	cf->argc = argc;

	for (i = 0; i < argc; i++)
		cf->args[i] = args[i];
}

/*sets a cmdFiles in and out pipe info*/
void setCmdFileIO(cmdFile *cf, int inStage, int outStage,
				  	int inFd, int outFd) {
	cf->inStage = inStage;
	cf->outStage = outStage;
	cf->inFd = inFd;
	cf->outFd = outFd;
}

/********************* Funcs *********************/

/********************* Printing *********************/

/*prints the stages of the pipeline*/
void printPipeline(fileSet *fs) {
	int i;

	for (i = 0; i < fs->size; i++)
		printStage(&(fs->files[i]));
}

/*prints one stage of the pipeline*/
void printStage(cmdFile *cf) {
	int i;

	printf("\n--------\nStage %d: \"", cf->stage);

	for (i = 0; i < cf->argc; i++) 
		printf("%s ", cf->args[i]);

	printf("\"\n--------\ninput: ");

	if (cf->inStage != -1)
		printf("pipe from stage %d\n", cf->inStage);
	else 
		printf("original stdin\n");

	if (cf->outStage != -1)
		printf("pipe to stage %d\n", cf->outStage);
	else 
		printf("original stdout\n");

	printf("argc: %d\nargv: ", cf->argc);

	if (cf->argc) {
		for (i = 0; i < cf->argc - 1; i++)
			printf("\"%s\",", cf->args[i]);

		printf("\"%s\"\n", cf->args[cf->argc - 1]);
	} else printf("\n");
}
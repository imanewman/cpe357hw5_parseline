#include "parseline.h"

int main(int argc, char *argv[]) {
	char str[MAX_CMD_LEN];
	fileSet *fs;
	input *in;

	str[MAX_CMD_LEN - 1] = '\0';

	printf("line: ");
	
	if (!(fgets(str, MAX_CMD_LEN, stdin))) {
		perror("no input given\n");
		return 1;
	}

	if (str[MAX_CMD_LEN - 1] != '\0') {
		perror("command too long\n");
		return 1;
	}

	in = initInput(str);

	if ((inputErrorCheck(in))) {
		free(in);
		return 1;
	}
		
	fs = makeFileSet(in);

	printPipeline(fs);

	free(fs);
	free(in);

	return 0;
}
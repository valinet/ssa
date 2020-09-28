#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

const string ttyColours[] = {ANSI_COLOR_RESET,
	ANSI_COLOR_RED, ANSI_COLOR_GREEN, ANSI_COLOR_YELLOW,
    ANSI_COLOR_BLUE, ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN};

size_t read(string s)
{
	size_t strlens = 0, bytes;
	string temp = s;
	do {
		bytes = fread(temp, sizeof(char), READCHUNKSIZE, stdin);
		temp = temp + bytes;
		strlens += bytes;
	}
	while (bytes == READCHUNKSIZE);

	/*char* ret;
	ret = fgets(s, STRINGLENGTH, stdin);
	if (ret == NULL) {
		exit(-1);
	}*/
	fflush(stdin);
	strlens = strlen(s);
	if (s[strlens - 1] == '\n') {
		s[strlens - 1] = '\0';
		strlens--;
	}

	return strlens;
}

void print(string text, size_t strlens, int* matches, bool sameColour) {
	size_t t;
	for (t = 0; t < strlens; t++) {
		if (matches[t]) {
			if (sameColour) {
				printf("%s", ttyColours[1]);
			} else {
				printf("%s", ttyColours[matches[t] % TTY_COLOURS_SIZE]);
			}
		} else {
			printf("%s", ttyColours[0]);
		}
		putchar(text[t]);
	}
	printf("\n");
}

int readfile(string text, char*** words, int* wordsLength) {
	size_t i, j;
	if (freopen("ahocorasick.in", "r", stdin) == NULL) {
		return -1;
	}
	if (freopen("ahocorasick.out", "w", stdout) == NULL) {
		return -1;
	}
	if (scanf("%s", text) != 1) {
		return -1;
	}
	if (scanf("%d", wordsLength) != 1) {
		return -1;
	}
	(*wordsLength)++;
	(*words) = malloc((*wordsLength) * sizeof(char*));
	if ((*words) == NULL) {
		return -1;
	}
	for (i = 1; i < (*wordsLength); i++) {
		(*words)[i] = malloc(AHOPATTERNLENGTH * sizeof(char));
		if ((*words)[i] == NULL) {
			for (j = 0; j < i; j++) {
				free((*words)[j]);
			}
			free((*words));
			return -1;
		}
		if (scanf("%s", (*words)[i]) != 1) {
			return -1;
		}
	}
	return 0;
}

void printfile(int* matches, char*** words, int wordsLength) {
	size_t i;
	for (i = 1; i < wordsLength; i++) {
		printf("%d\n", matches[i]);
		free((*words)[i]);
	}
	free(*words);
	fclose(stdin);
	fclose(stdout);
}

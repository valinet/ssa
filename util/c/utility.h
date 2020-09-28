#ifndef UTILITY_H
#define UTILITY_H

#define STRINGLENGTH       2000000
#define AHOSTRINGLENGTH    1000000
#define PATTERNLENGTH      2000000
#define AHOPATTERNLENGTH   10002
#define READCHUNKSIZE      100

#define ANSI_COLOR_RED     "\x1b[1m\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[1m\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[1m\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[1m\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[1m\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[1m\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[1m\x1b[0m"

#define TTY_COLOURS_SIZE 7

#define TRUE 1
#define FALSE 0

typedef char* string;
typedef size_t bool;
typedef int Trie;

size_t read(string s);
void print(string text, size_t strlens, int* matches, bool sameColour);
int readfile(string text, char*** words, int* wordsLength);
void printfile(int* matches, char*** words, int wordsLength);

#endif /* UTILITY_H */

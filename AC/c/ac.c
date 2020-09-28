#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <limits.h> */

#include "../../util/c/queue.h"
#include "../../util/c/utility.h"

#ifdef INFOARENA
#include "../../util/c/queue.c"
#include "../../util/c/utility.c"
#endif

#define STATES 20000
#define SCHAR_MIN 0
#define SCHAR_MAX 26
#define MAXARGC 101
#define OFFSET 97

Trie trie[STATES][SCHAR_MAX + 1];
Trie output[STATES][MAXARGC];
Trie failure[STATES];
char text[STRINGLENGTH];
int matches[STRINGLENGTH];

static inline __attribute__((always_inline)) 
Trie* buildTrie(char** argv, int argc, int verbose, size_t* argvlen) {
    size_t i, j;
    Trie state = 1;
    Trie current = 0;
    Queue* queue;
    Trie stat, fail;
    int t;

    memset(trie, -1, sizeof(trie));

    for (i = 1 + verbose; i < argc; ++i) {
        current = 0;
        for (j = 0; j < argvlen[i]; j++) {
            
            t = ((unsigned char**)argv)[i][j] - OFFSET;

            if (trie[current][t] == (Trie)-1) {
                trie[current][t] = state;
                state++;
            }
            current = trie[current][t];
        }
        output[current][i - 1 - verbose] = 1;
    }

    for (i = SCHAR_MIN; i < SCHAR_MAX + 1; ++i) {
        if (trie[0][i] == (Trie)-1) {
            trie[0][i] = 0;
        }
    }

    queue = newQueue(sizeof(Trie));
    if (queue == NULL) {
        return NULL;
    }

    memset(failure, -1, sizeof(failure));
    for (i = SCHAR_MIN; i < SCHAR_MAX + 1; ++i) {
        if (trie[0][i] != 0) {
            failure[trie[0][i]] = 0;
            enqueue(queue, &trie[0][i]);
        }
    }

    while (queue->size) {
        stat = *(Trie*)(dequeue(queue));
        for (i = SCHAR_MIN; i < SCHAR_MAX + 1; ++i) {
            if (trie[stat][i] != (Trie)-1) {
                fail = failure[stat];
                while (trie[fail][i] == (Trie)-1) {
                    fail = failure[fail];
                }
                fail = trie[fail][i];
                failure[trie[stat][i]] = fail;
                for (j = 0; j < argc - verbose - 1; j++) {
                    if (output[fail][j] == 1) {
                        output[trie[stat][i]][j] = 1;
                    }
                }
                enqueue(queue, &trie[stat][i]);
            }
        }
    }

    destroy(queue);

    return *trie;
}

static inline __attribute__((always_inline)) 
Trie nextState(Trie currentState, Trie nextInput) {
    Trie stat = currentState;
    while (trie[stat][nextInput] == (Trie)-1) {
        stat = failure[stat];
    }
    return trie[stat][nextInput];
}

int ahocorasick(char** argv, int argc, string text, 
    int verbose, int sameColour, int fromFile) {
    size_t i, j, t, match = 0, strlentext;
    size_t* argvlen;
    bool found;
    Trie currentState;

    argvlen = malloc(argc * sizeof(size_t));
    if (argvlen == NULL) {
        return 0;
    }
    for (i = 1 + verbose; i < argc; i++) {
        argvlen[i] = strlen(argv[i]);
    }

    buildTrie(argv, argc, verbose, argvlen);
    currentState = 0;
    strlentext = strlen(text);

    for (i = 0; i < strlentext; i++) {
        currentState = nextState(currentState, text[i] - OFFSET);
        found = FALSE;
        for (j = 0; j < argc - verbose - 1; j++) {
            if (output[currentState][j] == 1) {
                found = j + 1 + verbose;
                break;
            }
        }
        if (found == FALSE) {
            continue;
        }
        for (j = 1 + verbose; j < argc; j++)
        {
            if (output[currentState][j - 1 - verbose] == 1) {
                if (!fromFile) {
                    if (verbose) {
                        printf("Word %s appears from %d to %d.\n", 
                            argv[j], (int)(i - argvlen[j] + 1), (int)i);
                    }
                    else {
                        for (t = i - argvlen[j] + 1; t <= i; t++) {
                            matches[t] = j - verbose - sameColour;
                        }
                    }
                } else {
                    matches[j]++;
                }
                match++;
            }
        }
    }
    free(argvlen);
    return match;
}

int main(int argc, char** argv) {
    char** words;
    int wordsLength;
    int strlens;
    bool verbose = FALSE, sameColour = FALSE;
    if (argc > 1) {
        strlens = read(text);
        ahocorasick(argv, argc, text, verbose = !strcmp(argv[1], "--verbose"),
            sameColour = !strcmp(argv[1], "--samecolour"), FALSE);
        if (!verbose) {
		    print(text, strlens, matches, sameColour);
        }
    } else {
        if (readfile(text, &words, &wordsLength)) {
            return -1;
        }
        ahocorasick(words, wordsLength, text, FALSE, FALSE, TRUE);
        printfile(matches, &words, wordsLength);
    }

    return 0;
}
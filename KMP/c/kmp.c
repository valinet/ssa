#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#endif

#include "../../util/c/utility.h"

#ifdef INFOARENA
#include "../../util/c/utility.c"
#endif

/* String-ul in care vom efectua cautarea si dimensiunea lui. */
char text[STRINGLENGTH];
size_t strlens;
/* Vectorul auxiliar specific algoritmului */
size_t out[PATTERNLENGTH];
/* Vectorul ce indica daca caracterul de pe pozitia i face parte dintr-un
	subsir - se foloseste atunci cand se face afisarea textului */
int matches[STRINGLENGTH];

/* Static inline necesita trecerea la C99 sau gnu89 */
static inline __attribute__((always_inline)) 
void precompute(string text, size_t strlens, size_t* out)
{
	size_t j, i;

	j = 0, i = 1;
	while (i < strlens) {
		if (text[j] != text[i])
		{
			if (j > 0)
			{
				j = out[j - 1];
				continue;
			}
			else j--;
		}
		j++;
		out[i] = j;
		i++;
	}
}

int main(int argc, char** argv)
{
	char** words;
    int wordsLength;
	/* Pattern-ul pe care il vom cauta in string si dimensiunea lui. */
	char* pattern;
	size_t strlenp;
	/* In modul verbose, aplicatia tipareste pe cate un rand perechi
	   de numere care semnifica inceputul si respectiv sfarsitul unui
	   match (folositor pentru comparatiile intre algoritmi). */
	bool verbose, sameColour;
	/* Contori clasici si o variabila care tine numarul de match-uri
	   Aceasta este tiparita ca ultima linie in modul verbose. */
	size_t i, j, t, k, match = 0;

	/* Setez vectorul de match-uri pe 0 */
	memset(matches, 0, sizeof(int) * STRINGLENGTH);
	/* Decid daca sunt sau nu in modul verbose. */
	verbose = (argc > 1 && !strcmp(argv[1], "--verbose"));
	/* Decid daca voi colora cu aceeasi culoare match-urile (stil grep) */
	sameColour = (argc > 1 && !strcmp(argv[1], "--samecolour"));

	if (argc > 1) {
		/* Calculez lungimea textului in care facem cautarea */
		strlens = read(text);
		wordsLength = argc;
		words = argv;
	} else {
		if (readfile(text, &words, &wordsLength)) {
            return -1;
        }
		strlens = strlen(text);
	}
	
	/* In lista de argumente am fiecare cuvant dupa care caut. Execut
	   algoritmul pentru fiecare din cuvinte. */
	for (k = 1 + verbose + sameColour; k < wordsLength; k++) {
		/* Pattern-ul curent este urmatorul argument din lista */
		pattern = words[k];
		strlenp = strlen(pattern);
		
		/* Precalculez vectorul */
		precompute(pattern, strlenp, out);

		if (verbose) {
			printf(">> %s\n", pattern);
		}
	
		j = 0, match = 0;
		for (i = 0; i <= strlens; i++) {
			if (text[i] == pattern[j]) {
				j++;
				if (j == strlenp) {
					if (argc > 1) {
						if (verbose) {
							printf("Word %s appears from %d to %d.\n", 
								pattern, (int)(i - strlenp + 1), (int)(i + 1));
						}
						else {
							for (t = i - strlenp + 1; t < i + 1; t++) {
								matches[t] = k - verbose - sameColour;
							}
						}
					} else {
						matches[k]++;
					}
					match++;
					j = out[j - 1];
				}
			} else {
				if (j != 0) {
					j = out[j - 1];
					i--;
				}
			}
	
		}
		
		if (verbose) {
			printf("Matches: %d\n", match);
		}
	}

	if (argc > 1) {
		/* Scrie textul cu cuvintele eventual gasite evidentiate. */
		if (!verbose) {
			print(text, strlens, matches, sameColour);
		}
	} else {
		printfile(matches, &words, wordsLength);
	}

	return 0;
}

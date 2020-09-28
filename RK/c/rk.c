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

#include <math.h>

#define BASE 3
#define PRIME 101

/* String-ul in care vom efectua cautarea si dimensiunea lui. */
char text[STRINGLENGTH];
size_t strlens;
/* Vectorul ce indica daca caracterul de pe pozitia i face parte dintr-un
	subsir - se foloseste atunci cand se face afisarea textului */
int matches[STRINGLENGTH];

/* Static inline necesita trecerea la C99 sau gnu89 */
static inline __attribute__((always_inline)) 
size_t hash(string text, int pos, int length)
{
	size_t S = 0, i;
	for (i = pos; i < length + pos; ++i) {
		S = (S * BASE + text[i]) % PRIME;
	}
	return S;
}

int power(int b, int e) {
    int ret = 1;
    size_t i = 0;
    for (i = 0; i < e; i++) {
        ret = (ret * b) % PRIME;
    }
    return ret;
}

int main(int argc, char** argv)
{
	char** words;
    int wordsLength;
	/* Pattern-ul pe care il vom cauta in string si dimensiunea lui. */
	char* pattern;
	size_t strlenp;
	/* Hash-ul pattern-ului, respectiv hash-ul curent al bucatii din
	   text analizata in mod curent */
	int hashp, H;
	/* Puterea maxima pe care o putem obtine, adica baza la puterea
	   lungimea sirului */
	size_t max_power;
	/* In modul verbose, aplicatia tipareste pe cate un rand perechi
	   de numere care semnifica inceputul si respectiv sfarsitul unui
	   match (folositor pentru comparatiile intre algoritmi). */
	bool verbose, sameColour;
	/* Contori clasici si o variabila care tine numarul de match-uri
	   Aceasta este tiparita ca ultima linie in modul verbose. */
	size_t i = 0, j, k, t, match = 0;
	/* Variabile temporare */
	bool was;

	/* Setez vectorul de match-uri pe 0 */
	memset(matches, 0, STRINGLENGTH * sizeof(int));
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

		pattern = words[k];
		strlenp = strlen(pattern);

		/* Calcularea hash-ului primului grup de litere (nu ma pot folosi de
		un hash precedent, deoarece calculez pentru prima oara. */
		H = hash(text, 0, strlenp);

		/* Calcularea hash-ului pattern-ului */
		hashp = hash(pattern, 0, strlenp);

		if (H == hashp) {
			int was = TRUE;
			for (j = 0; j < strlenp; j++)
			{
				if (text[j] != pattern[j]) {
					was = FALSE;
					break;
				}
			}
			if (was)
			{
				if (argc > 1) {
					if (verbose) {
						printf("Word %s appears from %d to %d.\n", 
								pattern, (int)(i), (int)(i + strlenp));
					}
					else {
						for (t = i; t < i + strlenp; t++) {
							matches[t] = k - verbose - sameColour;
						}
					}
				} else {
					matches[k]++;
				}
				match++;
			}
			was = FALSE;
		}

		max_power = power(BASE, strlenp - 1);

		if (verbose) {
			printf(">> %s\n", pattern);
		}

		for (i = 1; i <= strlens - strlenp; i++) {
        	/*H = (((((H - text[i - 1] * max_power) % PRIME) + PRIME)
            	* BASE) + text[i + strlenp - 1]) % PRIME;*/
								H = H - text[i - 1] * max_power;
				H = H * BASE;
				H = H + text[i + strlenp - 1];
				H = H % PRIME;
				if (H < 0) H = H + PRIME;
			if (H == hashp) {
				was = TRUE;
				for (j = 0; j < strlenp; j++)
				{
					/*printf("%c %c\n", text[i + j], pattern[j]);*/
					if (text[i + j] != pattern[j]) {
                        was = FALSE;
                        break;
                    }
				}
				if (was)
				{
					if (argc > 1) {
						if (verbose) {
							printf("Word %s appears from %d to %d.\n", 
									pattern, (int)(i), (int)(i + strlenp));
						}
						else {
							for (t = i; t < i + strlenp; t++) {
								matches[t] = k - verbose - sameColour;
							}
						}
					} else {
						matches[k]++;
					}
					match++;
				}
				was = FALSE;
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

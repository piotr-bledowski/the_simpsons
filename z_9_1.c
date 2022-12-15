#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE   512
#define STR_NO_MAX 10000

#define OCENA 1

struct Word {
    char* word;
    int count;
};

// funkcja do sprowadzania wyrazów do postaci z samymi małymi literami
// potrzebne do porównywania wyrazów przy ostatnim sortowaniu
char* lower_string(char* input_str) {
	char* s = (char*)malloc(strlen(input_str) + 1);
	strcpy(s, input_str);

	for (int i = 0; i < strlen(s); i++) {
		s[i] = tolower(s[i]);
	}
	return s;
}

// funkcja porównawcza dla bsearch()
// porównuje nowo wczytany wyraz z wyrazami zawartymi w strukturach Word w tablicy
int cmpBsearch(const void *a, const void *b) {
    return strcmp(a, ((struct Word*) b)->word);
}

// funkcja porównawcza dla qsort()
// SORTUJE WZGLĘDEM KOLEJNOŚCI ALFABETYCZNEJ WYRAZÓW
int cmpWords(const void *a, const void *b) {
    // trzeba porównać pola word (stringi) 2 zmiennych typu struct Word
    // porównanie całych struct Word nie ma sensu
    return strcmp(((struct Word*) a)->word, ((struct Word*) b)->word);
}

// TA FUNKCJA SORTUJE WZGLĘDEM LICZBY WYSTĄPIEŃ WYRAZU
int cmpCounts(const void *a, const void *b) {
    // żeby nie pisać rzutowania wiele razy, dla czytelności
    struct Word* aa = (struct Word*) a;
    struct Word* bb = (struct Word*) b;
    
    // jeżeli są wyrazy o takich samych krotnościach, będą ułożone w kolejności alfabetycznej względem siebie
    if (aa->count == bb->count) {
        char* aa_lower = lower_string(aa->word);
        char* bb_lower = lower_string(bb->word);
        // zanim zwrócimy wartość r, trzeba wyczyścić aa_lower i bb_lower
        // bo funkcja lower_string alokuje pamięć, ale jej nie czyści
        int r = strcmp(aa_lower, bb_lower);
        
        free(aa_lower);
        free(bb_lower);
        
        return r;
    }
    else // bb - aa zamiast aa - bb żeby uzyskać kolejność malejącą
        return bb->count - aa->count;
}

int main(void) {
    int repet_no;        // numer wyrazu liczona wg liczby powtórzeń
    
    char buffer[BUF_SIZE];
    struct Word words[STR_NO_MAX];
    
    FILE *plik;
    plik = OCENA ? fopen("tekst_linux.txt","r") : stdin;
    if(plik==NULL) { printf("Blad w fopen\n");  exit(1); }
    
    if(OCENA==0) printf("Który (liczac od 1) wyraz ma byc wyprowadzony na ekran (wg malejącej krotności wyrazow): ");
    scanf("%d",&repet_no);

    int size = 0;
    
    // zapewnienie, że buffer jest pusty
    memset(buffer, 0, sizeof(buffer));
    
    while (!feof(plik)) {
        char c = fgetc(plik);
        
        // wczytywanie słowa
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '/' || c == '\'' || c == '-') {
            buffer[strlen(buffer)] = c;
        }
        // koniec słowa, jedno całe słowo wczytane
        else if ((c == ' ' || c == '\n') && strlen(buffer) > 0) {
            char *tmp = (char*) malloc(strlen(buffer) + 1);
            strcpy(tmp, buffer);
            
            // tmp to nowy wczytany wyraz
            struct Word* word = (struct Word*) bsearch(tmp, words, size, sizeof(struct Word), cmpBsearch);
            
            if (word != NULL) { // taki wyraz już się pojawił
                word->count++;
                free(tmp);
            }
            else { // pierwsze wystąpienie
                words[size].word = tmp;
                words[size].count = 1;
                size++;
                // sortowanie względem kolejności alfabetycznej wyrazów
                qsort(words, size, sizeof(struct Word), cmpWords); // trzeba utrzymywać tablicę posortowaną, żeby funkcja bsearch działała
            }
            
            // wyczyszczenie buffera przed wczytaniem do niego następnego wyrazu
            memset(buffer, 0, sizeof buffer);
        }
    }
    
    // sortowanie względem liczby powtórzeń wyrazów
    qsort(words, size, sizeof(struct Word), cmpCounts);
    
    printf("%s %d", words[repet_no].word, words[repet_no].count);
    
    return 0;
}

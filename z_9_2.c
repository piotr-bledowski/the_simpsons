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
    struct Word* next;
};

// funkcja do sprowadzania wyrazów do postaci z samymi małymi literami
// przyda się przy porównywaniu
char* lower_string(char* input_str) {
	char* s = (char*)malloc(strlen(input_str) + 1);
	strcpy(s, input_str);

	for (int i = 0; i < strlen(s); i++) {
		s[i] = tolower(s[i]);
	}
	return s;
}

// funkcja do dodania nowego elementu do listy (we właściwym alfabetycznie miejscu)
void push(struct Word** head, char* word) {
    struct Word *new_word;
    new_word = (struct Word*) malloc(sizeof(struct Word));
    new_word->word = word;
    new_word->count = 1;
    new_word->next = NULL;

    struct Word* curr = *head;
    struct Word* prev = NULL;

    while (curr != NULL) {
        // wartości lower_string trzeba przenieść do zmiennych żeby zwolnić pamięć po mallocu wywołanym w funkcji lower_string()
        char* s1 = lower_string(curr->word);
        char* s2 = lower_string(new_word->word);
        if (strcmp(s1, s2) > 0) {
            // nowe słowo powinno się znaleźć między słowami w prev i curr
            new_word->next = curr;
            if (prev != NULL)
                prev->next = new_word;
            else // jeśeli lista zawiera tylko 1 element, nie można operować na 2 elementach (prev i curr), więc to szczególny przypadek
                *head = new_word;
            free(s1);
            free(s2);
            return; // słowo dodane do listy, wyjście z funkcji
        }
        else if (strcmp(s1, s2) < 0) { // nowe słowo powinno znaleźć się po słowie w prev (dalej w liście)
            prev = curr;
            curr = curr->next;
        }
        free(s1);
        free(s2);
    }

    // jeżeli kod dochodzi do tego miejsca, to znaczy, że nie znaleziono w liście wyrazu
    // który powinien być w kolejności leksykograficznej po nowym
    // innymi słowy, nowy wyraz jest w tej kolejności ostatni w liście
    if (prev == NULL) // jeśli lista jest pusta
        *head = new_word;
    else
        prev->next = new_word;
}

// funkcja sprawdzająca czy słowo jest już w liście
struct Word* search(struct Word** head, char* searched_word) {
    struct Word* curr = *head;

    while (curr != NULL) {
        if (strcmp(searched_word, curr->word) == 0) // znaleziono słowo w liście
            return curr;
        curr = curr->next;
    }

    // nie znaleziono
    return NULL;
}

// funkcja zwracająca n-te słowo z listy
struct Word* nth_word(struct Word** head, int n) {
    int i = 1;
    struct Word* curr = *head;

    while (curr != NULL) {
        if (i == n)
            return curr;
        i++;
        curr = curr->next;
    }

    return NULL;
}

int main(void) {
    char buffer[BUF_SIZE];
    struct Word* head = NULL;

    int n;

    FILE *plik;
    plik = OCENA ? fopen("tekst_linux.txt","r") : stdin;
    if(plik==NULL) { printf("Blad w fopen\n");  exit(1); }

    if(OCENA==0) printf("Który (liczac od 1) wyraz ma byc wyprowadzony na ekran: ");
    scanf("%d",&n);

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
            struct Word* word = (struct Word*) search(&head, tmp);

            if (word != NULL) { // taki wyraz już się pojawił
                word->count++;
                free(tmp);
            }
            else { // pierwsze wystąpienie
                push(&head, tmp);
            }

            // wyczyszczenie buffera przed wczytaniem do niego następnego wyrazu
            memset(buffer, 0, sizeof buffer);
        }
    }

    struct Word* word_to_print = nth_word(&head, n);

    if (word_to_print != NULL)
        printf("%s %d\n", word_to_print->word, word_to_print->count);

    return 0;
}

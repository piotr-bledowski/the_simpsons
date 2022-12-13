#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_RECURSION_DEPTH 4

// argumenty i odpowiadające im wartości, które były już obliczane przez funkcję podcałkową
// są przechowywane w liście
struct node {
    int arg;
    int val;
    struct node *next;
};

// dodanie pary wartości do listy
void push(struct node *head, int arg, int val) {
    struct node *new_node;
    new_node = (struct node*) malloc(sizeof(struct node));

    new_node->arg = arg;
    new_node->val = val;
    new_node->next = head;
    head = new_node;
}

// jeżeli dla tego argumentu funkcja była już wywołana, zostanie zwrócony zapisany wynik
// jeżeli jeszcze nie była, zostanie zwrócona wartość NAN
float has_been_calculated(float arg, struct node* head) {
    struct node* curr = head;
    
    while (curr != NULL) {
        if (curr->arg == arg)
            return curr->val;
        curr = curr->next;
    }
    
    return NAN;
}

float f(float x) {
    return 1.0/((x - 0.5) * (x - 0.5) + 0.01);
}

// float f(float x) {
//     return x*x*x*x - 5*x*x*x + 10*x*x + 3*x + 1;
// }

float simpson(float a, float b, struct node* head) {
    float f1, f2, f3;
    
    float c = (a + b) / 2;
    
    f1 = has_been_calculated(a, head);
    f2 = has_been_calculated(c, head);
    f3 = has_been_calculated(b, head);
    
    // jeżeli nie liczono jeszcze wartości dla a
    if (isnan(f1)) {
        f1 = f(a);
        push(head, a, f1);
    }
    
    // jeżeli nie liczono jeszcze wartości dla c
    if (isnan(f2)) {
        f2 = f(c);
        push(head, c, f2);
    }
    
    // jeżeli nie liczono jeszcze wartości dla b
    if (isnan(f3)) {
        f3 = f(b);
        push(head, b, f3);
    }
    
    // wzór Simpsona
    return (float)fabs(a - b) * (f1 + 4.0*f2 + f3) / 6.0;
}

float rec(float a, float b, float s, float delta, int depth, struct node* head) {
    if (depth > MAX_RECURSION_DEPTH)
        return NAN;
    
    float c = (a + b) / 2;
    
    float s1 = simpson(a, c, head);
    float s2 = simpson(c, b, head);

    // jeśli funkcja obliczyła mniej więcej dokładną wartość, zwraca ją, jeśli nie, będzie wywoływana dalej
    // aż do zwrócenia mniej więcej dokładnej wartości lub przekroczenia limitu głębokości rekurencji kiedy zwróci NAN
    
    if (fabs(s1 + s2 - s) <= delta) {
        return s1 + s2;
    }
    
    return rec(a, c, s1, delta, depth + 1, head) + rec(c, b, s2, delta, depth + 1, head);
}

int main(void) {
    int visited_len = 1;
    
    // maksymalnie 2^MAX_RECURSION_DEPTH wywołań funkcji simpson()
    for (int i = 0; i < MAX_RECURSION_DEPTH; i++) {
        visited_len *= 2;
    }
    
    // maksymalnie 3 nowe elementy w tablicy na każde wywołanie funkcji simpson()
    visited_len *= 3;
    
    float visited[visited_len];
    
    for (int i = 0; i < visited_len; i++)
        visited[i] = NAN;
        
    int a, b;
    float delta;
    
    struct node* head = NULL;
    
    printf("a: ");
    scanf("%d", &a);
    printf("b: ");
    scanf("%d", &b);
    printf("delta: ");
    scanf("%f", &delta);
    
    float s = simpson(a, b, head);
    
    float simp = rec(a, b, s, delta, 1, head);
    
    if (isnan(simp))
        printf("\nNAN\n");
    else
        printf("\nwartosc calki na przedziale [a, b]: %f\n", simp);
    
    return 0;
}

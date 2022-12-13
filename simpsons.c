#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_RECURSION_DEPTH 4 // trzeba zarezerwować 3 * 2^MAX_RECURSION_DEPTH pamięci na tablicę do memoizacji

float f(float x) {
    return 1.0/((x - 0.5) * (x - 0.5) + 0.01);
}

// float f(float x) {
//     return x*x*x*x - 5*x*x*x + 10*x*x + 3*x + 1;
// }

// jeżeli dla tego argumentu funkcja była już wywołana, zostanie zwrócony zapisany wynik
// jeżeli jeszcze nie była, zostanie zwrócona wartość NAN
float has_been_calculated(float arg, float visited[], int len) {
    for (int i = 0; i < len; i++) {
        if (visited[i] == arg)
            return arg;
        if (isnan(visited[i]))
            return NAN;
    }
    return NAN;
}

float simpson(float a, float b, float visited[], int len, int* ind) {
    float f1, f2, f3;
    
    float c = (a + b) / 2;
    
    f1 = has_been_calculated(a, visited, len);
    f2 = has_been_calculated(c, visited, len);
    f3 = has_been_calculated(b, visited, len);
    
    if (isnan(f1)) {
        f1 = f(a);
        visited[*ind] = f1;
        (*ind)++;
    }
    
    if (isnan(f2)) {
        f2 = f(c);
        visited[*ind] = f2;
        (*ind)++;
    }
    
    if (isnan(f3)) {
        f3 = f(b);
        visited[*ind] = f3;
        (*ind)++;
    }
    
    return (float)fabs(a - b) * (f1 + 4.0*f2 + f3) / 6.0;
}

float rec(float a, float b, float s, float delta, int depth, float visited[], int len, int* ind) {
    if (depth > MAX_RECURSION_DEPTH)
        return NAN;
    
    float c = (a + b) / 2;
    
    float s1 = simpson(a, c, visited, len, ind);
    float s2 = simpson(c, b, visited, len, ind);

    // jeśli funkcja obliczyła mniej więcej dokładną wartość, zwraca ją, jeśli nie, będzie wywoływana dalej
    // aż do przekroczenia limitu głębokości rekurencji kiedy zwróci NAN
    
    if (fabs(s1 + s2 - s) <= delta) {
        return s1 + s2;
    }
    
    return rec(a, c, s1, delta, depth + 1, visited, len, ind) + rec(c, b, s2, delta, depth + 1, visited, len, ind);
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
        
    int a, b, ind;
    float delta;
    
    ind = 0;
    
    printf("a: ");
    scanf("%d", &a);
    printf("b: ");
    scanf("%d", &b);
    printf("delta: ");
    scanf("%f", &delta);
    
    float s = simpson(a, b, visited, visited_len, &ind);
    
    float simp = rec(a, b, s, delta, 1, visited, visited_len, &ind);
    
    if (isnan(simp))
        printf("\nNAN\n");
    else
        printf("\nwartosc calki na przedziale [a, b]: %f\n", simp);
    
    return 0;
}

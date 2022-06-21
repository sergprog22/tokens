#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE (int)1e6 + 6
#define N 1000
int whitespace(char c) {
    return (c == ' ') || (c == '\t') || (c == '\n') || (c== '\r') ;
}
int digit(char c) {
    return '0' <= c && c <= '9';
}
int sign(char c) {
    if (c == '+') return 1;
    if (c == '-') return -1;
    return 0;
}

int readInt(char *s, int *len, int *x) {
    int i = 0;
    int sgn;
    if (digit(s[i])) {
        sgn = 1;
    } else {
        sgn = sign(s[i]);
        if (!sgn) {
            return 0;
        }
        ++i;
    }
    *x = 0;
    int mem_i = i;
    while(digit(s[i])) {
        *x = 10 * (*x) + s[i] - '0';
        ++i;
    }
    *len = i;
    *x *= sgn;
    return (i != mem_i);
}
int parse(char *s, int len, int *a, int *n) {
    *n = 0;
    int pos = 0;
    if (whitespace(s[pos])) ++pos;
    int x, delta;
    while(pos < len) {
        if (!readInt(s + pos, &delta, &x)) {
            return 0;
        }
        a[(*n)++] = x;
        pos += delta;
        if (whitespace(s[pos])) ++pos;
    }
    if (whitespace(s[pos])) ++pos;
    return (pos == len) && (1 <= *n) && (*n <= N);
}
int a[N];
int max_sub[N][N];
int calculated[N][N];
int opt[N][N];
void relax(int l, int r, int sum, int step) {
    if (max_sub[l][r] < sum) {
        max_sub[l][r] = sum;
        opt[l][r] = step;
    }
}
int calculate(int l, int r) {
    if (l > r) {
        return 0;
    }
    if (l == r) {
        max_sub[l][r] = a[l];
        opt[l][r] = 1;
        return max_sub[l][r];
    }
    if (!calculated[l][r]) {
        max_sub[l][r] = a[l] - calculate(l + 1, r);
        opt[l][r] = 1;
        relax(l, r, a[r] - calculate(l, r - 1), 2);
        if (r - l + 1 >= 2) {
            relax(l, r, a[l] + a[l + 1] - calculate(l + 2, r), 3);
            relax(l, r, a[r] + a[r - 1] - calculate(l, r - 2), 4);
            relax(l, r, a[l] + a[r] - calculate(l + 1, r - 1), 5);
        }
        calculated[l][r] = 1;
    }
    return max_sub[l][r];
}

char otherPlayer(char player) {
    if (player == 'A') return 'B';
    return 'A';
}
void reconstruct(int l, int r, char player) {
    if (r < l)
        return;

    printf("%c ", player);
    player = otherPlayer(player);

    switch(opt[l][r]) {
        case 1:
            printf("%d (%d)\n", l, a[l]);
            reconstruct(l + 1, r, player);
            break;
        case 2:
            printf("%d (%d)\n", r, a[r]);
            reconstruct(l, r - 1, player);
            break;
        case 3:
            printf("%d, %d (%d, %d)\n", l, l + 1, a[l], a[l + 1]);
            reconstruct(l + 2, r, player);
            break;
        case 4:
            printf("%d, %d (%d, %d)\n", r, r - 1, a[r], a[r - 1]);
            reconstruct(l, r - 2, player);
            break;
        case 5:
            printf("%d, %d (%d, %d)\n", l, r, a[l], a[r]);
            reconstruct(l + 1, r - 1, player);
            break;
    }
}

int main() {
    printf("Zetony:\n");
    char s[BUFFER_SIZE];
    fgets(s,N+1,stdin);
    int len = strlen(s);
    int n;
    if (!parse(s, len, a, &n)) {
        printf("Nespravny vstup.\n");
        return 0;
    }
    calculate(0, n - 1);
    reconstruct(0, n - 1, 'A');

    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
    }

    int A = (max_sub[0][n - 1] + sum) / 2;
    int B = sum - A;
    printf("A: %d, B: %d\n", A, B);

    return 0;
}
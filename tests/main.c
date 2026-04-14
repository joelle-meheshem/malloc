#include <stdio.h>
#include "my_malloc.h"

int main() {
    printf("Starting malloc test...\n");

    int *a = (int *)my_malloc(sizeof(int) * 5);

    for (int i = 0; i < 5; i++) {
        a[i] = i * 10;
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    my_free(a);

    int *b = (int *)my_malloc(sizeof(int) * 3);
    int *c = (int *)my_malloc(sizeof(int) * 3);

    b[0] = 1;
    c[0] = 2;

    printf("b: %d, c: %d\n", b[0], c[0]);

    my_free(b);
    my_free(c);

    printf("Done!\n");

    return 0;
}
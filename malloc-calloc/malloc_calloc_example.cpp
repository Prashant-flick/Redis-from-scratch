#include <stdio.h>
#include <stdlib.h>

int main() {
    // malloc only allocate memory and not set any values so we will get garbage values
    int *arr = (int*) malloc(5 * sizeof(int));

    for (int i=0; i<5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);

    // calloc allocates memory and also called memset to set all the values to zero
    arr  = (int*) calloc(5, sizeof(int));

    for (int i=0; i<5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
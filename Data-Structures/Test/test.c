#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main()
{
    printf("Hello.\n");
    int a = 3;
    int* b = &a;
    printf("The value of b is: %d\n", *b);

}
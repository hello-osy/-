#include <stdio.h>
#include <stdbool.h>

typedef struct addition
{
    bool positive;
    int beforeNum;
    int afterNum;
    struct num* before;
    struct num* after;
} addition, *addition;

char* readFile(FILE* input);
addition Add(addition firstNum, addition secondNum);

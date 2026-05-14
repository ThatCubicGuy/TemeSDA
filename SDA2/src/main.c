#include "search_index/google.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc > 1) printf("%lu", string_Hash(argv[1]));
    return 1;
}

#include <stdio.h>

unsigned long string_Hash(string source)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *(source++))) hash = ((hash << 5) + hash) + c;
    return hash;
}

int main(int argc, char **argv)
{
    if (argc > 1) printf("%lu", string_Hash(argv[1]));
    return 1;
}

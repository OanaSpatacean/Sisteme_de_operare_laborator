#include <stdio.h>

int main(int argc,char **argv)
{
    if (argc != 2)
    {
        perror("Usage: <grupa>");
        return -1;
    }

    printf("Grupa: %s\n",argv[1]);

    printf("Oana\n");
    printf("Mara\n");
    printf("Dan\n");
    printf("Iasmina\n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

struct stat fisierStat;

int main(int argc, char *argv[]) 
{
    const char *fisierInNume = argv[1];
    const char *fisierOutNume = argv[2];
    char ch = argv[3][0];
    int fisierIn;
    int fisierOut;
    int NumarLitereMici = 0;
    int NumarLitereMari = 0;
    int NumarCifre = 0;
    int NumarAparitiiCaracter = 0;
    char buffer[1];
    char statsInfo[256];

    if (argc != 4) 
    {
        perror("Argumente insuficiente");
        exit(-1);
    }

    fisierIn = open(fisierInNume, O_RDONLY);

    if (fisierIn == -1) {
        perror("Eroare la deschiderea fisierului de intrare");
        exit(-1);
    }

    fisierOut = open(fisierOutNume, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fisierOut == -1) {
        perror("Eroare la deschiderea fisierului de iesire");
        close(fisierIn);
        exit(-1);
    }

    while (read(fisierIn, buffer, 1) > 0) 
    {
        char c = buffer[0];

        if (islower(c)) 
        {
            NumarLitereMici++;
        } 
        if (isupper(c)) 
        {
            NumarLitereMari++;
        } 
        if (isdigit(c)) 
        {
            NumarCifre++;
        }
        if (c == ch) 
        {
            NumarAparitiiCaracter++;
        }
    }

    int size = sprintf(statsInfo, "Numar litere mici: %d\nNumar litere mari: %d\nNumar cifre: %d\nNumar aparitii caracter: %d\n",
                      NumarLitereMici, NumarLitereMari, NumarCifre, NumarAparitiiCaracter);

    write(fisierOut, statsInfo, size);

    if (fstat(fisierIn, &fisierStat) == -1) 
    {
        perror("Eroare la obtinerea informatiilor despre fisier");
        close(fisierIn);
        close(fisierOut);
        exit(-1);
    }

    size = sprintf(statsInfo, "Dimensiune: %ld\n", fisierStat.st_size);

    write(fisierOut, statsInfo, size);

    close(fisierIn);
    close(fisierOut);

    return 0;
}

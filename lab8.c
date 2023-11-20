#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void procesulTatalui()
{
    char caracter;
    int PID;
    int status;

    while (1)
    {
        scanf("%c", &caracter);
        usleep(50000); 

        if (caracter == 'c')
        {
            break;
        }
    }

    PID = wait(&status);

    if (WIFEXITED(status))
    {
        printf("Procesul tatalui %d s-a finalizat cu statusul %d\n", PID, WEXITSTATUS(status));
    }
}

void procesulFiului()
{
    execl("./student", "student", "grupa6.1", NULL);
    perror("Eroare la executia execl");
    exit(-1);
}

int main(void)
{
    int PID = fork();

    printf("%d\n", PID);

    if (PID < 0)
    {
        perror("Eroare la citirea PID-ului");
        exit(-1);
    }
    else if (PID > 0)
    {
        procesulTatalui();
    }
    else if (PID == 0)
    {
        procesulFiului();
    }

    return 0;
}

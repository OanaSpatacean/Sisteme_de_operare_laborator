#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void procesulTatalui(int pipe_fd)
{
    char caracter;
    int status;

    while (1)
    {
        read(pipe_fd, &caracter, sizeof(char));
        usleep(50000); 

        if (caracter == 'c')
        {
            break;
        }
    }

    close(pipe_fd);

    wait(&status);

    if (WIFEXITED(status))
    {
        printf("Procesul tatalui s-a finalizat cu statusul %d\n", WEXITSTATUS(status));
        exit(status);
    }
}

void procesulFiului(int pipe_fd)
{
    close(pipe_fd);

    execl("./student", "student", "grupa6.1", NULL);
    perror("Eroare la executia execl");
    exit(-1);
}

int main(void)
{
    int PID = fork();
    int pipe_fd[2];

    printf("%d\n", PID);

    if(pipe(pipe_fd) == -1)
    {
        perror("Eroare la crearea pipe-ului");
        exit(-1);
    }

    if (PID < 0)
    {
        perror("Eroare la citirea PID-ului");
        exit(-1);
    }
    else if (PID > 0)
    {
        close(pipe_fd[1]);
        procesulTatalui(pipe_fd[0]);
    }
    else if (PID == 0)
    {
        close(pipe_fd[0]);
        procesulFiului(pipe_fd[1]);
    }

    return 0;
}
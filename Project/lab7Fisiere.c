#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

struct stat fisierStat;

void scrie_permisiuni_user(mode_t mode, char buffer[]) 
{
    char permisiune[4];

    if(mode & S_IRUSR)
    {
        permisiune[0] = 'R';
    }
    else
    {
        permisiune[0] = '-';
    }

    if(mode & S_IWUSR)
    {
        permisiune[1] = 'W';
    }
    else
    {
        permisiune[1] = '-';
    }

    if(mode & S_IXUSR)
    {
        permisiune[2] = 'X';
    }
    else
    {
        permisiune[2] = '-';
    }

    permisiune[3] = '\0';

    sprintf(buffer, "drepturi de acces user: %s\n", permisiune);
}

void scrie_permisiuni_grup(mode_t mode, char buffer[]) 
{
    char permisiune[4];

    if(mode & S_IRGRP)
    {
        permisiune[0] = 'R';
    }
    else
    {
        permisiune[0] = '-';
    }

    if(mode & S_IWGRP)
    {
        permisiune[1] = 'W';
    }
    else
    {
        permisiune[1] = '-';
    }

    if(mode & S_IXGRP)
    {
        permisiune[2] = 'X';
    }
    else
    {
        permisiune[2] = '-';
    }

    permisiune[3] = '\0';

    sprintf(buffer, "drepturi de acces grup: %s\n", permisiune);
}

void scrie_permisiuni_altii(mode_t mode, char buffer[1024]) 
{
    char permisiune[4];

    if(mode & S_IROTH)
    {
        permisiune[0] = 'R';
    }
    else
    {
        permisiune[0] = '-';
    }

    if(mode & S_IWOTH)
    {
        permisiune[1] = 'W';
    }
    else
    {
        permisiune[1] = '-';
    }

    if(mode & S_IXOTH)
    {
        permisiune[2] = 'X';
    }
    else
    {
        permisiune[2] = '-';
    }

    permisiune[3] = '\0';

    sprintf(buffer, "drepturi de acces altii: %s\n", permisiune);
}
 
int main(int argc, char **argv) 
{
    char *argFisierIn = argv[1];
    int fisierIn = 0;
    int fisierOut = 0;
    int lungime = 0;
    int inaltime = 0;
    struct passwd *userInfo;
    struct group *grupInfo;
    char timp[30];
    char header[100];
    char buffer[1024];
    char buffer2[1024];
    char buffer3[1024];
    char buffer4[1024];

    if (argc != 2) 
    {
        perror("Usage: %s <fisier_intrare>\n");
        return -1;
    }
    
    if (stat(argFisierIn, &fisierStat) == -1) 
    {
        perror("Eroare la obtinerea informatiilor din fisierul de intrare");
        return -1;
    }

    fisierIn = open(argFisierIn, O_RDONLY);
    
    if (fisierIn == -1) 
    {
        perror("Eroare la deschiderea fisierului de intrare");
        return -1;
    }

    lseek(fisierIn, 18, SEEK_SET);
    read(fisierIn, &inaltime, 4);
    read(fisierIn, &lungime, 4);
    strftime(timp, sizeof(timp), "%d.%m.%Y", localtime(&fisierStat.st_mtime));

    sprintf(header, "statistica.txt");

    fisierOut = open(header, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fisierOut == -1) 
    {
        perror("Eroare la crearea fisierului de iesire statistica.txt");
        return -1;
    }
    
    userInfo = getpwuid(fisierStat.st_uid);
    grupInfo = getgrgid(fisierStat.st_gid);

    sprintf(buffer, "nume fisier: %s\ninaltime: %d\nlungime: %d\ndimensiune: %ld\nidentificatorul utilizatorului: %s\ntimpul ultimei modificari: %s\ncontorul de legaturi: %lu\n", argFisierIn, inaltime, lungime, fisierStat.st_size, userInfo->pw_name, timp, fisierStat.st_nlink);

    write(fisierOut, buffer, strlen(buffer));

    if (userInfo != NULL) 
    {
        scrie_permisiuni_user(fisierStat.st_mode, buffer2);
        write(fisierOut, buffer2, strlen(buffer2));
    }

    if (grupInfo != NULL) 
    {
        scrie_permisiuni_grup(fisierStat.st_mode,buffer3);
        write(fisierOut, buffer3, strlen(buffer3));
    }


    if (grupInfo != NULL) 
    {  
        scrie_permisiuni_altii(fisierStat.st_mode,buffer4);
        write(fisierOut, buffer4, strlen(buffer4));
    }

    close(fisierIn);
    close(fisierOut);

    return 0;
}

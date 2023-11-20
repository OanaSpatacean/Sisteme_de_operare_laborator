#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

struct stat fisierStat;
struct dirent *intrare;

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

    sprintf(buffer, "drepturi de acces altii: %s\n\n", permisiune);
}

void citeste_lungime_inaltime_bmp(char *caleDirector, char *numeFisier, int *lungime, int *inaltime)
{
    char caleFisier[1024];
    int fisierBMP;

    sprintf(caleFisier, "%s/%s", caleDirector, numeFisier);

    fisierBMP = open(caleFisier, O_RDONLY);

    if (fisierBMP == -1)
    {
        perror("Eroare la deschiderea fisierului BMP");
        return;
    }

    lseek(fisierBMP, 18, SEEK_SET);
    read(fisierBMP, inaltime, sizeof(int));
    read(fisierBMP, lungime, sizeof(int));

    close(fisierBMP);
}

void extrage_info(char *cale) 
{
    struct stat fisierStat;
    char caleFisier[1024];
    int fisierOut;
    int lungime = 0;
    int inaltime = 0;
    struct passwd *userInfo;
    struct group *grupInfo;
    char caleaTarget[256];
    char buffer[1024];
    char buffer2[1024];
    char buffer3[1024];
    char buffer4[1024];
    char buffer5[1024];

    sprintf(caleFisier, "%s/%s", cale, intrare->d_name);

    if (lstat(caleFisier, &fisierStat) == -1) 
    {
        perror("Eroare la obtinerea informatiilor din fisierul de intrare");
        return;
    }

    fisierOut = open("statistica.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if (fisierOut == -1) 
    {
        perror("Eroare la deschiderea fisierului de iesire statistica.txt");
        return;
    }

    if (S_ISREG(fisierStat.st_mode)) //fișier obișnuit
    {
        sprintf(buffer, "nume fisier: %s\n", intrare->d_name);

        write(fisierOut, buffer, strlen(buffer));

        if (strstr(intrare->d_name, ".bmp")) 
        {
            citeste_lungime_inaltime_bmp(cale, intrare->d_name, &lungime, &inaltime);

            sprintf(buffer, "inaltime: %d\nlungime: %d\n", inaltime, lungime);
            write(fisierOut, buffer, strlen(buffer));
        } 
        else 
        {
            sprintf(buffer, "dimensiune: %ld\n", fisierStat.st_size);
            write(fisierOut, buffer, strlen(buffer));
        }

        userInfo = getpwuid(fisierStat.st_uid);
        grupInfo = getgrgid(fisierStat.st_gid);

        sprintf(buffer, "identificatorul utilizatorului: %s\ntimpul ultimei modificari: %scontorul de legaturi: %lu\n", userInfo->pw_name, ctime(&fisierStat.st_mtime), fisierStat.st_nlink);

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
    } 
    else if (S_ISLNK(fisierStat.st_mode)) //legatura simbolica
    {    
        if (readlink(caleFisier, caleaTarget, sizeof(caleaTarget)) == -1) 
        {
            perror("Eroare la citirea legaturii simbolice");
            return;
        }

        userInfo = getpwuid(fisierStat.st_uid);
        grupInfo = getgrgid(fisierStat.st_gid);

        sprintf(buffer, "nume legatura: %s\ndimensiune legatura: %ld\n",intrare->d_name, fisierStat.st_size);
        
        write(fisierOut, buffer, strlen(buffer));

        sprintf(buffer5, "target: %s\n", caleaTarget);
        write(fisierOut, buffer5, strlen(buffer5));

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
    } 
    else if (S_ISDIR(fisierStat.st_mode)) //director
    {
        userInfo = getpwuid(fisierStat.st_uid);
        grupInfo = getgrgid(fisierStat.st_gid);

        sprintf(buffer, "nume director: %s\nidentificatorul utilizatorului: %s\ndrepturi de acces user: ",intrare->d_name, userInfo->pw_name);
        
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
    }

    close(fisierOut);
}

int main(int argc, char **argv) 
{
    char *directorIn = argv[1];
    DIR *dir;
    
    if (argc != 2) 
    {
        perror("Usage: %s <director_intrare>\n");
        return -1;
    }

    dir = opendir(directorIn);

    if (dir == NULL) 
    {
        perror("Eroare la deschiderea directorului de intrare");
        return -1;
    }

    while ((intrare = readdir(dir)) != NULL) 
    {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0) 
        {
            extrage_info(directorIn);
        }
    }

    closedir(dir);

    return 0;
}
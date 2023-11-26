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
#include <sys/wait.h>

struct stat fisierStat;
struct dirent *intrare;
char buffer[1024];
int liniiScrise = 0;
int nrCopii = 0;

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

void citeste_lungime_inaltime_bmp(char *caleIn, char *numeFisier, int *lungime, int *inaltime)
{
    char caleFisier[1024];
    int fisierBMP;

    sprintf(caleFisier, "%s/%s", caleIn, numeFisier);

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

void covertireGri(int fisierIn, int lungime, int inaltime) 
{
    unsigned char pixel[3]; // fiecare pixel are 3 componente (roșu, verde, albastru)
    unsigned char pixel_gri;
    double P_gri;

    lseek(fisierIn, 0, SEEK_SET);
    for (int i = 0; i < 54; i++) //mută cursorul la începutul imaginii (după header)
    {
        read(fisierIn, buffer, sizeof(char));
    }

    // Parcurge fiecare pixel și aplică formula pentru conversia la nivel de gri
    for (int i = 0; i < inaltime; i++) 
    {
        for (int j = 0; j < lungime; j++) 
        {
            read(fisierIn, pixel, sizeof(pixel));

            // Calculul intensității gri folosind formula specificată
            P_gri = 0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0];

            // Convertirea la tipul unsigned char (1 octet)
            pixel_gri = (unsigned char)P_gri;

            //mută cursorul înapoi la locația pixelului original
            lseek(fisierIn, -3, SEEK_CUR);

            // Scrierea pixelului în noul fișier
            write(fisierIn, &pixel_gri, sizeof(char)); //rosu
            write(fisierIn, &pixel_gri, sizeof(char)); //verde
            write(fisierIn, &pixel_gri, sizeof(char)); //albastru
        }
    }

    close(fisierIn);
}

void procesulCeluiDeAlDoileaCopilFisierBMP(int fisierIn, int lungime, int inaltime) 
{
    covertireGri(fisierIn, lungime, inaltime);
    exit(0); //succes
}

void procesulTataluiPtCopilulDoiFisierBMP(int PID)
{
    int status;

    waitpid(PID, &status, 0); //asteapta sa se termine procesul copilui

    if (WIFEXITED(status)) 
    {
        printf("S-a încheiat procesul cu PID-ul %d și codul %d. Fisier bmp.\n", PID, WEXITSTATUS(status));
    }
}

void procesCopilFisierObisnuitContinut(int pipe_fd1, char *caleFisier)
{
    dup2(pipe_fd1, 1);
    execlp("cat", "cat", caleFisier, NULL); //acest proces va trebui sa genereze folosind o comanda conținutul fișierului obișnuit fără extensie .bmp pentru care a extras informațiile statistice
    exit(-1);
}

void procesTataFisierObisnuitContinut(int PID)
{
    int status;

    waitpid(PID, &status, 0);

    if (WIFEXITED(status)) 
    {
        printf("S-a încheiat procesul cu PID-ul %d și codul %d. Fisier obisnuit -> continut.\n", PID, WEXITSTATUS(status));
    }
}

void procesCopilFisierObisnuitPropozitii(int pipe_fd0, char *caracter)
{
    dup2(pipe_fd0, 0);
    execlp("bash", "bash", "shellscript.sh", caracter, NULL); //va calcula, din secvența primită de la procesul fiu responsabil cu generarea conținutului fișierului, numărul de propoziții corecte
    perror("Eroare la executarea script-ului");
    exit(-1);
}

void procesTataFisierObisnuitPropozitii(int PID, int n, char *caracter)
{
    int status;

    waitpid(PID, &status, 0);

    if (WIFEXITED(status)) 
    {
        n = n + WEXITSTATUS(status);
        printf("S-a încheiat procesul cu PID-ul %d și codul %d. Fisier obisnuit -> propozitii.\n", PID, WEXITSTATUS(status));
        printf("--> Au fost identificate in total %d propozitii corecte care contin caracterul %s.\n", n, caracter);
    }
}

void extrageInfo(char *caleIn, char *caleOut, char *caracter) 
{
    struct stat fisierStat;
    char caleFisier[1024];
    int fisierOut;
    int fisierIn;
    char caleFisierOut[1024];
    int lungime = 0;
    int inaltime = 0;
    struct passwd *userInfo;
    struct group *grupInfo;
    char caleaTarget[256];
    char buffer2[1024];
    char buffer3[1024];
    char buffer4[1024];
    char buffer5[1024];
    int n = 0; //nr propozitii corecte
    int pipe_fd[2];

    sprintf(caleFisier, "%s/%s", caleIn, intrare->d_name);
    sprintf(caleFisierOut, "%s/%s_statistica.txt", caleOut, intrare->d_name);

    if (lstat(caleFisier, &fisierStat) == -1) 
    {
        perror("Eroare la obtinerea informatiilor din fisierul de intrare");
        return;
    }

    fisierOut = open(caleFisierOut, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if (fisierOut == -1) 
    {
        perror("Eroare la deschiderea fisierului de iesire statistica.txt");
        return;
    }

    if (S_ISREG(fisierStat.st_mode)) //fișier obișnuit
    {
        sprintf(buffer, "nume fisier: %s\n", intrare->d_name);
        write(fisierOut, buffer, strlen(buffer));

        fisierIn = open(caleFisier, O_RDWR);

        if (fisierIn == -1) 
        {
            perror("Eroare la deschiderea fisierului de intrare");
            return;
        }

        if (strstr(intrare->d_name, ".bmp")) 
        {
            citeste_lungime_inaltime_bmp(caleIn, intrare->d_name, &lungime, &inaltime);

            sprintf(buffer, "inaltime: %d\nlungime: %d\n", inaltime, lungime);
            write(fisierOut, buffer, strlen(buffer));
            liniiScrise = liniiScrise + 10;

            //covertirea grayscale
            int convertPID = fork(); // pentru fiecare intrare ce reprezinta o imagine .bmp procesul părinte va crea un al doilea proces 

            if (convertPID < 0) 
            {
                perror("Eroare la crearea procesului fiu");
            } 
            else if (convertPID == 0) 
            {
                procesulCeluiDeAlDoileaCopilFisierBMP(fisierIn, lungime, inaltime);
            }
            else if (convertPID > 0) 
            {
                procesulTataluiPtCopilulDoiFisierBMP(convertPID);
            }
        } 
        else // pentru categoria fișierelor obișnuite care nu au extensia .bmp
        {
            sprintf(buffer, "dimensiune: %ld\n", fisierStat.st_size);
            write(fisierOut, buffer, strlen(buffer));
            liniiScrise = liniiScrise + 8;

            if (pipe(pipe_fd) == -1)
            {
                perror("Eroare la crearea pipe-ului");
                exit(-1);
            }

            int continutPID = fork(); //părintele va mai crea cate un proces fiu

            if (continutPID < 0) 
            {
                perror("Eroare la crearea procesului fiu");
            } 
            if (continutPID == 0)  //procesul copil 
            {
                close(pipe_fd[0]); //inchide capatul de citire a pipe-ului           
                procesCopilFisierObisnuitContinut(pipe_fd[1], caleFisier);
            } 
            else if (continutPID > 0) //procesul parinte 
            {
                procesTataFisierObisnuitContinut(continutPID);
                close(pipe_fd[1]); //inchide capatul de scriere a pipe-ului
            } //va trimite acest conținut celuilalt proces fiu creat de părinte

            int numarPropPID = fork(); //Cel de-al doilea proces fiu creat de părinte pentru fiecare fișier obișnuit fără extensia .bmp

            if (numarPropPID < 0) 
            {
                perror("Eroare la crearea procesului fiu");
            } 
            if (numarPropPID == 0) //procesul copil 
            {
                close(pipe_fd[1]); //inchide capatul de scriere a pipe-ului   
                procesCopilFisierObisnuitPropozitii(pipe_fd[0], caracter);
            } 
            else if (numarPropPID > 0) //procesul parinte 
            {
                close(pipe_fd[0]); //inchide capatul de citire a pipe-ului
                close(pipe_fd[1]); //inchide capatul de scriere a pipe-ului
                //va transmite acest număr părintelui printr-un pipe
                procesTataFisierObisnuitPropozitii(numarPropPID, n, caracter);
            }
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

        liniiScrise = liniiScrise + 6;
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

        liniiScrise = liniiScrise + 5;
    }

    close(fisierOut);
}

void procesulPrimuluiCopil(char *caleIn, char *caleOut, int *liniiScrise, char *caracter) 
{
    extrageInfo(caleIn, caleOut, caracter);
    exit(*liniiScrise); //La încheierea fiecărui proces fiu, acesta va trimite părintelui numărul de linii scrise în fișierul statistica.txt.
}

void procesulTataluiPtPrimulCopil(int nrCopii, int PID, int statusIesire[]) 
{
    int status;

    // Așteptarea proceselor copil si colectarea exit status
    for (int i = 0; i < nrCopii; ++i) 
    {
        PID = waitpid(-1, &status, 0);

        if (WIFEXITED(status)) 
        {
            statusIesire[i] = WEXITSTATUS(status);
            liniiScrise += statusIesire[i];
            printf("S-a încheiat procesul cu PID-ul %d și codul %d. Nr de linii scrise în fișierul statistica.txt este %d\n", PID, statusIesire[i], liniiScrise);
        }
    }
}

int main(int argc, char **argv) 
{
    char *directorIn = argv[1];
    char *directorOut = argv[2];
    char *caracter = argv[3];
    DIR *dir;
    int PID;
    int statusIesire[1024];  // array sa stochez exit status la fiecare copil
    
    if (argc != 4) 
    {
        perror("Folosire: <director_intrare> <director_iesire> <c>");
        return -1;
    }

    dir = opendir(directorIn);

    if (dir == NULL) 
    {
        perror("Eroare la deschiderea directorului de intrare");
        return -1;
    }

    while ((intrare = readdir(dir)) != NULL) //Pentru fiecare intrare (fișier obișnuit, fișier.bmp, director, legătură simbolică ce indică spre un fișier obișnuit) din directorul de intrare
    {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0) 
        {
            PID = fork(); // procesul părinte va crea câte un nou proces

            if (PID == 0) //procesul copil
            {
                procesulPrimuluiCopil(directorIn, directorOut, &liniiScrise, caracter);
            } 
            else if (PID < 0) // eroare
            {
                perror("Eroare la crearea procesului copil");
            } 
            else 
            {
                statusIesire[nrCopii++] = 0;  // initializarea statusului de iesire a fiecarui fiu
            }
        }
    }

    procesulTataluiPtPrimulCopil(nrCopii, PID, statusIesire);
    
    closedir(dir);

    return 0;
}
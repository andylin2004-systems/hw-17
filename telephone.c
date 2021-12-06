#include <stdio.h>
#include <strings.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#define SEMKEY 42069
#define MEMKEY 69420

int main()
{
    int *data;
    int semd, shmd, r, v, file;
    semd = semget(SEMKEY, 1, 0);
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;

    semop(semd, &sb, 1);

    char fromFileContent[1024];
    struct stat fileInfo;
    stat("telephone.txt", &fileInfo);
    char newIn[1024];

    file = open("telephone.txt", O_RDONLY, 0644);

    close(file);
    file = open("telephone.txt", O_WRONLY | O_APPEND, 0644);
    read(STDIN_FILENO, newIn, sizeof(char) * 1024);
    newIn[strlen(newIn) - 2] = '\0';
    write(file, newIn, sizeof(char) * strlen(newIn));

    sb.sem_op = 1;
    semop(semd, &sb, 1);
}
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
    int *lastLineLength;
    int semd, shmd, file;
    semd = semget(SEMKEY, 1, 0);
    shmd = shmget(MEMKEY, 1, 0);
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;

    semop(semd, &sb, 1);
    lastLineLength = shmat(shmd, 0, 0);

    struct stat fileInfo;
    stat("telephone.txt", &fileInfo);
    char newIn[1024] = {0};
    char *fromFileContent = malloc(fileInfo.st_size);

    if (fileInfo.st_size){
        file = open("telephone.txt", O_RDONLY, 0644);
        lseek(file, *lastLineLength * -1 * sizeof(char), SEEK_END);
        read(file, fromFileContent, fileInfo.st_size);
        printf("Last line:\n%sNew line:\n", fromFileContent);
        close(file);
    }else{
        printf("Here's to the next chapter of a new story!\nNew line:\n");
    }

    file = open("telephone.txt", O_WRONLY | O_APPEND, 0644);
    read(STDIN_FILENO, newIn, sizeof(char) * 1024);
    write(file, newIn, sizeof(char) * strlen(newIn));
    *lastLineLength = strlen(newIn);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    return 0;
}
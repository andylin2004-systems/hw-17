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
#define KEY 24601

int main(int argc, char* argv[]){
    int *data;
    int shmd;
    int semd;
    if (argc > 1)
    {
        if (strcmp(argv[1], "create") == 0){
            shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0640);
            data = shmat(shmd, 0, 0);
            *data = open("telephone.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
            semd = semget(KEY + 1, 1, IPC_CREAT);
        }
        else if (strcmp(argv[1], "remove") == 0)
        {
            struct stat fileInfo;
            stat("telephone.txt", &fileInfo);
            shmdt(data);
            shmctl(shmd, IPC_RMID, 0);
            char *story = malloc(fileInfo.st_size);
            write(*data, story, fileInfo.st_size);
        }
    }
}
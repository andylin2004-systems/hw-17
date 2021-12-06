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
#include <errno.h>
#define SEMKEY 42069
#define MEMKEY 69420

// union semun{
//     int val;
//     struct semid_ds *buf;
//     unsigned short *array;
//     struct seminfo *__buf;
// };

int main(int argc, char *argv[])
{
    int semd, memd, v, r, file;
    char input[3];

    if (argc > 1)
    {
        if (strcmp(argv[1], "remove") == 0)
        {
            file = open("telephone.txt", O_RDONLY, 0644);
            semd = semget(SEMKEY, 0, 0);
            memd = shmget(MEMKEY, 0, 0);
            semctl(semd, 0, IPC_RMID);
            shmctl(memd, IPC_RMID, NULL);

            struct stat filestat;
            stat("telephone.txt", &filestat);
            char *fileContent = malloc(filestat.st_size);
            read(file, fileContent, filestat.st_size);
            printf("%s\n", fileContent);
            close(file);
        }
        else if (strcmp(argv[1], "create") == 0)
        {
            semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0640);
            memd = shmget(MEMKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0640);
            if (semd == -1){
                printf("%s\n", strerror(semd));
            }else{
                union semun us;
            us.val = 1;
            semctl(semd, 0, SETVAL, us);
            file = open("telephone.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
            close(file);
            }
        }else{
            printf("Use args 'create' or 'remove' to use this.");
        }
    }else{
        printf("Use args 'create' or 'remove' to use this.");
    }
}
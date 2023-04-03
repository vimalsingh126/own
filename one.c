/*	
Name -		 Vimal Singh
Batch - ECEP 22002
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SMSIZE 100

int share_mem_write(char *buff);
int share_mem_read(char *buff);

int main()
{

    int fd[2];
    pid_t pid;

    char buff[255];
    int len, shmid, ch;
    char *shm;
    key_t key;

    printf("1. write\n2. read\nEnter your choice: ");
    scanf("%d", &ch);
    getchar();
    if (ch == 2)
    {
        shm = shmat(shmid, NULL, 0);  // attaching to  shared memory

        // Do error check
        if(shm < 0)
        {
            perror("shmat");
            exit(0);
        }
        else
        {
            share_mem_read(buff);
            shmdt(shm);

        }
        return 0;
    }

    else
    {
        //create pipe
        pipe(fd);

        //create a child
        pid = fork();

        //child forking
        if (-1 == pid)
        {
            // indicates the fork was unsuccessful
            perror("fork\n");
            exit(1);
        }   
        else if (0 == pid)
        {
            //child

            //close unwanted end
            close(fd[1]);

            //read the data from fd
            read(fd[0], &len, sizeof(len));
            read(fd[0], buff, len);   

            //write reversed string to shared memory
            share_mem_write(buff);   
        }
        else
        {
            //parent process

            //enter the string that you want to save
            printf("Enter the string: ");
            fgets(buff, 255, stdin);

            //strcpy(buff, "Hello");       
            len = strlen(buff);
            len++; //to account for null

            //close unwanted fd
            close(fd[0]);

            //write the data into this fd
            write(fd[1], &len, sizeof(len));
            write(fd[1], buff, len);

            sleep(2);

            //read from shared memory
            share_mem_read(buff);
        }
    }
    return 0;
}   

int share_mem_write(char *buff)
{
    int shmid, i, len;
    char *shm, *shm1;
    key_t key;

    //key
    key = 'A';

    //create shared memory
    shmid = shmget(key, SMSIZE, IPC_CREAT | 0666);

    //check if created
    if(shmid < 0)
    {
        perror("shmget");
        exit(1);
    }   
    else
    {
        shm = shmat(shmid, NULL, 0);
        shm1 = shm;

        len = strlen(buff);

        //write to shared memory
        for(i = 0; i <= len; i++)
        {
            *shm1 = toupper(*buff);
            shm1++;
            buff++;
        }       
    }
    return 0;

}

int share_mem_read(char *buff)
{
    int shmid, i, len;
    char *shm, *shm1;
    key_t key;

    //key
    key = 'A';

    //create shared memory
    shmid = shmget(key, SMSIZE, 0666);

    //check if created
    if(shmid < 0)
    {
        perror("shmget");
        exit(1);
    }   

    shm = shmat(shmid, NULL, 0);
    if(shm < 0)
    {
        perror("shmat");
        exit(0);
    }
    else
    {   
        printf("value from shared memory: %s\n", shm);
        shm1 = shm;
        len = strlen(shm1);

        //read from shared memory
        for(i = 0; i <= len; i++)
        {
            *buff = *shm1;
            shm1++;
            buff++;
        }       
    }
    return 0;
}
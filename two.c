/*	
Name -		 Vimal Singh
Batch - ECEP 22002
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SMSIZE 100 //shared memory size

int reverse_str(char *str, int len);
int share_mem_write(char *buff);
int share_mem_read(char *buff);

int main()
{
    int shmid, len, i;
    char *shm, *shm1, num;
    key_t key;
    char buff[200];

    key = 'A';

    shmid = shmget(key, SMSIZE, 0666);  // create shared memory segment

    // Do error check
    if(shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    shm = shmat(shmid, NULL, 0);  // attaching to shared memory

    // Do error check
    if(shm < 0)
    {
        perror("shmat");
        exit(0);
    }
    else
    {
        share_mem_read(buff);
        reverse_str(buff, strlen(buff));
        printf("After reverse: \n");
        share_mem_write(buff);

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
    printf("value from shared memory: %s\n", shm);
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

int reverse_str(char *str, int len)
{
        int i;
        char string[10];
        char *str1 = string;

        for(i = 0; i <= len-1; i++)
        {
                str1[i] = str[len - 1 - i];
        }

        for(i = 0; i <= len-1; i++)
        {
                str[i] = str1[i];
        }
        str[i] = '\0';
}

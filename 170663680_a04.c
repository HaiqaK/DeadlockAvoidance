/*
------------------------------
Project: cp386a04
File: 170663680_a04.c
------------------------------
Author: Haiqa Khurshid
GitHub: HaiqaK
Repsository: https://github.com/HaiqaK/CP386_A04
------------------------------
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h> 
#define FILE_NAME "sameple4_in.txt"
#define MAX_INPUT_SIZE 256

int *available;     // number available resources  
int **maximum;      // max number of processes 
int **allocated;    // amount allocated per customer
int **need;         // customer need
int customer;
int resource;
int *sequence;


int **readFile(char* fileName);
void *threadRun(void *t);
int *safetySeq();
void spd(int *data, int a);         // single pointer data
void dpd(int **data, int a, int b); // double pointer dadta

int **readFile(char *fileName)
{
        FILE *in = fopen(fileName, "r");
    if (!in)
    {
        printf("Child A: Error in opening input file... exiting with error code -1\n");
        return NULL;
    }

    struct stat st;
    fstat(fileno(in), &st);
    char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
    fileContent[0] = '\0';
    while(!feof(in))
    {
        char line[100];
        if(fgets(line, 100, in) != NULL)
        {
            strncat(fileContent, line, strlen(line));
        }
    }
    fclose(in);

    char *command = NULL;
    char *fileCopy = (char *) malloc((strlen(fileContent) + 1) * sizeof(char));
    strcpy(fileCopy, fileContent);
    command = strtok(fileCopy, "\r\n");
    while(command != NULL)
    {
        customer++;
        command = strtok(NULL, "\r\n");
    }
    strcpy(fileCopy, fileContent);
    char *lines[customer];
    int i = 0;
    command = strtok(fileCopy, "\r\n");
    while (command != NULL)
    {
        lines[i] = malloc(sizeof(command) * sizeof(char));
        strcpy(lines[i], command);
        i++;
        command = strtok(NULL, "\r\n");
    }
    int **maximum = malloc(sizeof(int *) * customer);
    for (int j = 0; j < customer; j++)
    {
        int *temp = malloc(sizeof(int) * resource);
        char *token = NULL;
        int k = 0;
        token = strtok(lines[j], ",");
        while (token != NULL)
        {
            temp[k] = atoi(token);
            k++;
            token = strtok(NULL, ",");
        }
        maximum[j] = temp;
    }
    return maximum;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Needs more parameters...\n");
        return -1;
    }
    resource = argc -1;

    // available array initialization 
    available = malloc(sizeof(int) + resource);
    for(int i = 1; i < argc; i++)
    {
        available[i-1] = atoi(argv[i]);
    }

    // max array initialization 
    maximum = readFile(FILE_NAME); // read file, assign values

    // allocated array 
    allocated = malloc(sizeof(int *) * customer);
    for(int i = 0; i < customer; i++){
        allocated[i] = malloc(sizeof(int) * resource);
    }

    // need array
    need = malloc(sizeof(int *) * customer);
    for (int i = 0; i < customer; i++)
    {
        need[i] = malloc(sizeof(int) * resource);
    }

    int s = 0; // initializing safe (s) as 0 

    char *usersInput = malloc(sizeof(char) * MAX_INPUT_SIZE);

    printf("Number of Customers: %d\n", customer);
    printf("Currently Available Resources: ");
    spd(available, resource);
    printf("Maximum Resourcs from File: \n");
    dpd(maximum, customer, resource);

    // will run till users input is exit
    while(1)
    {
        printf("Enter Command: ");
        fgets(usersInput, MAX_INPUT_SIZE, stdin);

        if(strlen(usersInput) > 0 && usersInput[strlen(usersInput)-1] == '\n')
        {
            usersInput[strlen(usersInput) - 1] = '\0';
        }

        if (strstr(usersInput, "RQ"))
        {
            int count = 0;
            int *array = malloc(sizeof(int) * (resource + 1));  // input array
            char *token = NULL;
            token = strtok(usersInput, " ");
            while(token != NULL)
            {
                if (count > 0)
                {
                    array[count - 1] = atoi(token);
                }
                token = strtok(NULL, " ");
                count++;
            }

            // customer to allocated insert into allocaed array
            int customerAllocated = array[0]; 
            if(customerAllocated < customer && count == resource + 2)
            {
                for(int i = 0; i < resource; i++)
                {
                    allocated[customerAllocated][i] = array[i + 1];
                    need[customerAllocated][i] = maximum[customerAllocated][i] - allocated[customerAllocated][i];
                    // need has to be positive
                    if(need[customerAllocated][i] < 0)
                    {
                        need[customerAllocated][i] = 0;
                    }
                }
            }
            else 
            {
                if(customerAllocated >= customer)
                {
                    printf("Thread invalid, try again.\n");
                }
                else
                {
                    printf("Needs more paramenters, try again.\n");
                }
            }
            free(array);
            sequence = safetySeq();
            printf("REquest is Satisfied\n");
            if(sequence[0] == -1)
            {
                s = 0;
                printf("* Unsafe state *\n");
            }
            else
            {
                s = 1;
                printf("State is now safe.\n");
            }
        }
        else if(strstr(usersInput, "RL"))
        {
            int count = 0;
            int *array = malloc(sizeof(int) * (resource + 1));
            char *token = NULL;
            token = strtok(usersInput, " ");
            while(token != NULL)
            {
                if (count > 0)
                {
                    array[count - 1] = atoi(token);
                }
                token = strtok(NULL, " ");
                count++;
            }

            int customerAllocated = array[0];
            // removing allocated array
            if (customerAllocated < customer && count == resource + 2)
            {
                for(int i = 0; i < resource ; i++)
                {
                    if (array[i+1] <= allocated[customerAllocated][i])
                    {
                        allocated[customerAllocated][i] -= array[i+1];
                        need[customerAllocated][i] = maximum[customerAllocated][i] - allocated[customerAllocated][i];
                    }
                    else 
                    {
                        printf("cannot release more resources than allocated.\n");
                        break;
                    }
                }
            }
            else 
            {
                if (customerAllocated >= customer)
                {
                    printf("Threads invalid, try again\n");
                }
                else 
                {
                    printf("Needs more paramenters, try again.\n");
                }
            }
            free(array);
            sequence = safetySeq();
            printf("Request satisfied\n");
            if(sequence[0] == -1)
            {
                s = 0;
                printf("* Unsafe state *\n");
            }
            else
            {
                s = 1;
                printf("State is now safe.\n");
            }
        }
        else if (strstr(usersInput, "*"))
        {
            printf("Allocated Resources: \n");
            dpd(allocated, customer, resource);

            printf("Needed: \n");
            dpd(need, customer, resource);

            printf("Available: \n");
            spd(available, resource);

            printf("Maximum Resources:\n");
            dpd(maximum, customer, resource);

        }
        else if(strstr(usersInput, "Run"))
        {
            sequence = safetySeq();
            if (s == 1)
            {
                for(int i = 0; i < customer; i++)
                {
                    int runThread = sequence[i];
                    pthread_t tid;
                    pthread_attr_t attr;
                    pthread_attr_init(&attr);
                    pthread_create(&tid, &attr, threadRun, (void *) &runThread);
                    pthread_join(tid, NULL);
                }
            }
            else 
            {
                printf("* Unsafe state *\n");
            }
        }
        else if (strstr(usersInput, "exit"))
        {
            free(available);
            free(maximum);
            free(allocated);
            free(need);
            free(sequence);
            return 0;
        }
        else 
        {
            printf("\"%s\" is not a valid input, enter one of [\"RQ\",\"RL\",\"*\",\"Run\",\"exit\"].\n", usersInput);
        }
    }
    return 0; 
}

void *threadRun(void *t)
{
   int *thread = (int *)t; // thread id
   // print what would show after command 'Run'
   printf("--> Customer/Thread %d\n", *thread);
   
   printf("     Allocated Resources: ");
   // allocaed resource output
   for (int i = 0; i < resource ; i++) {
       printf("%d ", allocated[*thread][i]);
   } 
   printf("\n");

   printf("     Needed: ");
   // needed output
   for(int i = 0; i < resource; i++) {
       printf("%d ", need[*thread][i]);
   }
    printf("\n");

   printf("     Available: ");
   // avaiable resources outoput
   for (int i = 0; i < resource; i ++){
       printf("%d ", available[i]);
   }
   printf("\n");

   printf("     Thread has started\n");
   sleep(1);

   printf("     Thread has finished\n");
   sleep(1);

   printf("     Thread is releasing resources\n");
   printf(1);
   
   printf("     New Available: ");
   // new available resources outout
   for (int i = 0; i < resource; i++){
       available[i] += allocated[*thread][i];
       printf("%d ", available[i]);
   }
   printf("\n\n");
   sleep(1);

   pthread_exit(NULL);
}

int *safetySeq(){   // function is used to contain safety algroithm that will be used when calling parts of program

    int *done = malloc(sizeof(int) * customer);
    int *sequence = malloc(sizeof(int) * customer);
    int *wrok = malloc(sizeof(int) * resource);
    for(int i = 0; i < resource; i ++){     // what is avialble to work
        wrok[i] = available[i];
    }

    int count = 0;
    while(count < customer){
        int safe = 0;
        for (int i = 0; i < customer; i++){
            if(done[i] == 0){
                int safety = 1;
                for (int j = 0; j < resource; j++){
                    if (need[i][j] > wrok[j]){
                        safety = 0;
                        break;
                    }
                }
                if (safety == 1){
                    done[i] = 1;
                    sequence[count] = i;
                    count++;
                    safe = 1;

                    for(int j = 0; j < resource; j++){
                        wrok[j] += allocated[i][j];
                    }
                }
            }
        }
            if(safe == 0){
                for(int m = 0; m < customer; m++){
                    sequence[m] = -1;
                }
                free(wrok);
                free(done);
                return sequence;
        }
    }
    free(wrok);
    free(done);
    return sequence;
}

void spd(int *data, int a)
{
    for (int i = 0; i < a; i++){
        printf("%d", data[i]);
        if ( i < a -1)
            printf(" ");
    }
    printf("\n");
}

void dpd(int **data, int a, int b)
{
    for (int i = 0; i < a; i ++){
        for (int j = 0; j < b; j ++){
            printf("%d", data[i][j]);
            if(j < b -1)
                printf(" ");
        }
        printf("\n");
    }
}

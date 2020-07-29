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

int *available; // number available resources  
int **maximum; // max number of processes 
int **allocated; // amount allocated per customer
int **need; // customer need
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
        return -1;
    }

    struct stat st;
    fstat(fileno(in), &st);
    char *fileContent = (char *)mallock(((int)st.st_size + 1) * sizeof(char));
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
        command = strtok(NULL, "\r\n"):
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
            token = strtok(NULL, ",")
        }
        maximum[j] = temp;
    }
    return maximum;
}

int main(int argc, char *argv[])
{
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
   
   printf("     Needed: ");
   // needed output
   for(int i = 0; i < resource; i++) {
       printf("%d ", need[*thread][i]);
   }

   printf("     Available: ");
   // avaiable resources outoput
   for (int i = 0; i < available; i ++){
       printf("%d ", available[*thread][i]);
   }

   printf("     Thread has started\n");
   printf("     Thread has finished\n");
   printf("     Thread is releasing resources\n");

   printf("     New Available: ");
   // new available resources outout
   for (int i = 0; i < resource; i++){
       available[*thread][i] += allocated[*thread][i];
       printf("%d ", available[*thread][i]);
   }

   pthread_exit(NULL);
}

void spd(int *data, int a)
{
}

void dpd(int **data, int a, int b)
{
}

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


int readFile(char* fileName);
void *threadRun(void *t);
int *safetySeq();
void spd(int *data, int a);         // single pointer data
void dpd(int **data, int a, int b); // double pointer dadta

int **readFile(char *fileName)
{
}

int main(int argc, char *argv[])
{
}

void *threadRun(void *t)
{
}

void spd(int *data, int a)
{
}

void dpd(int **data, int a, int b)
{
}

#ifndef  THREADS_H_INCLUDED
#define THREADS_H_INCLUDED

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "lists.h"
#include "helpers.h"

// thread structure
typedef struct {
    pthread_barrier_t* barrier;
    pthread_mutex_t* mutex;

    long long* id;
    FILE* file;
    int* nrReducers;
    int* nrMappers;

    node* perfectPowers[1000][1000];
    node* perfectPowersReducers[1000];
} thread_data;

// mapper function
void* mapper_f(void* arg) {
    thread_data data = *(thread_data*)arg;
    char buff[255];
    int number;
    while (1) {
        // use mutex to make sure only one thread reads from file at a time
        pthread_mutex_lock(data.mutex);
        // read from first file
        if (!fgets(buff, 255, data.file)) {
            // if end of file, unlock mutex and break
            pthread_mutex_unlock(data.mutex);
            break;
        }
        pthread_mutex_unlock(data.mutex);
        // remove newline character
        buff[strcspn(buff, "\n")] = 0;

        // initialize variables from reading from in.txt files
        char numberBuff[255];
        FILE* ptrIn;
        ptrIn = fopen(buff, "r");
        if (!ptrIn) {
            printf("Nu s-a putut deschide fisierul %s", buff);
        }
        fgets(numberBuff, 255, ptrIn);
        number = atoi(numberBuff);
        while (fgets(numberBuff, 255, ptrIn)) {
            number = atoi(numberBuff);
            // check if number is perfect power and add it to its corresponding list
            checkIfPerfectPower(number, *data.nrReducers, *data.id, data.perfectPowers);
        }
        fclose(ptrIn);
    }
    // barrier to make sure all mappers have finished
    pthread_barrier_wait(data.barrier);
    pthread_exit(NULL);
}

// reducer function
void* reducer_f(void* arg) {
    thread_data data = *(thread_data*)arg;
    // barrier to make sure all mappers have finished before reducers start
    pthread_barrier_wait(data.barrier);
    long long id = *data.id - *data.nrMappers;

    // add all perfect powers from mapper lists to reducer list by exponent alone
    for (int j = 0; j < *data.nrReducers; j++) {
        concatenateLists(&data.perfectPowersReducers[id], &data.perfectPowers[j][id]);
    }
    // remove duplicates from every reducer list
    removeDuplicates(&data.perfectPowersReducers[id]);
    // count remaining number of perfect powers in every reducer list
    long long nr = countList(data.perfectPowersReducers[id]);

    // determine output file name
    char fileName[255];
    sprintf(fileName, "out%lld.txt", id + 2);
    FILE* ptrOut;
    ptrOut = fopen(fileName, "w");
    // write number of perfect powers to output file
    fprintf(ptrOut, "%lld", nr);
    fclose(ptrOut);

    pthread_exit(NULL);
}

#endif
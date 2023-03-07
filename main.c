#include "headers/lists.h"
#include "headers/helpers.h"
#include "headers/threads.h"

int main(int argc, char* argv[]) {
    // declare thread synchronization primitives
    pthread_barrier_t barrier;
    pthread_mutex_t mutex;

    // declare and initialize variables
    FILE* ptr;
    char buff[255];
    int mappersNumber = atoi(argv[1]);
    int reducersNumber = atoi(argv[2]);
    int threadsNumber = mappersNumber + reducersNumber;

    // initialize thread synchronization primitives
    pthread_barrier_init(&barrier, NULL, threadsNumber);
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[threadsNumber];
    int r;
    long long id;
    void* status;
    long long ids[threadsNumber];

    // array of structs to be passed to threads
    thread_data* data = malloc(sizeof(thread_data) * threadsNumber);

    // 2d array of lists to store perfect powers structured by mapper id and power
    node* list[1000][1000];
    init2dArray(list, reducersNumber, mappersNumber);

    // array of lists to store perfect powers structured by power to store concatenation of lists
    node* reducersList[1000];
    initArray(reducersList, reducersNumber, mappersNumber);

    // open first file
    ptr = fopen(argv[3], "r");

    // read first line
    fgets(buff, 255, ptr);

    for (id = 0; id < threadsNumber; id++) {
        // initialize thread data
        ids[id] = id;
        data[id].mutex = &mutex;
        data[id].barrier = &barrier;
        data[id].id = &ids[id];
        data[id].file = ptr;
        data[id].nrReducers = &reducersNumber;
        data[id].nrMappers = &mappersNumber;

        // initialize arrays of lists
        for (int j = 0; j < reducersNumber; j++) {
            data[id].perfectPowersReducers[j] = reducersList[j];
        }
        for (int i = 0; i < mappersNumber; i++) {
            for (int j = 0; j < reducersNumber; j++) {
                data[id].perfectPowers[i][j] = list[i][j];
            }
        }
        // create thread
        // if id is less than mappersNumber, create mapper thread
        if (id < mappersNumber) {
            r = pthread_create(&threads[id], NULL, mapper_f, &data[id]);
        }
        // else create reducer thread
        else {
            r = pthread_create(&threads[id], NULL, reducer_f, &data[id]);
        }
        // check if thread was created successfully
        if (r) {
            printf("Eroare la crearea thread-ului %lld\n", id);
            exit(-1);
        }
    }

    for (id = 0; id < threadsNumber; id++) {
        // wait for thread to join
        r = pthread_join(threads[id], &status);

        // check if thread was joined successfully
        if (r) {
            printf("Eroare la asteptarea thread-ului %lld\n", id);
            exit(-1);
        }
    }

    // free synchronization primitives
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);

    // close file
    fclose(ptr);
    pthread_exit(NULL);
}
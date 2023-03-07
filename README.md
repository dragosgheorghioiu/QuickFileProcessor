# Tema 1 - APD

## General Description
The main thread starts a number of mapper threads and reducer threads provided as command line arguments.

## Mapper
Each mapper thread reads from 
a file and picks the file name of the file it will process. To make sure that each mapper thread will process a different file, a mutex is used. The critical section is the part of the code where the file name is read from the file. Then it will read line by line from the in.txt file and will check if the current number is a perfect power. If it is it will save the number in a 2D array of lists. The array will have as many lines as the number of reducer threads and the number of columns will be the number of mapper threads. If a mapper thread finishes processing a file, it will pick another file that has not been processed yet until there are no more files to process.
To make sure that each reducer thread will start its work only after all the mapper threads have finished, a barrier is used. The barrier will be initialized with the number of total threads. Each mapper thread will reach the barrier after all the in.txt files are processed. Then the reducer threads will start their work.

## Reducer
The reducer thread will reach the barrier before they start their work.
Each reducer thread will read from the 2D array of lists and will add them to its own list based on the power of the number. After all the numbers are added to the reducer lists, the reducer threads will remove any duplicates from their lists and will count the number of perfect powers for each power. Then the reducer threads will write the results in a out.txt file. 
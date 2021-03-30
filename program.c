/***************************
 * Renan Martins da Silva 
****************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define FILE_DATA_NAME "data.txt"
#define FILE_OPEN_FOR_WRITING "w"
#define FILE_OPEN_FOR_READING "r"
#define MIN_RANGE_VALUE 0
#define MAX_RANGE_VALUE 100


typedef enum { FALSE, TRUE } bool;


pthread_mutex_t mutex_file = PTHREAD_MUTEX_INITIALIZER;
pthread_t threadWriter, threadReader;


int getRandomValue()
{
    time_t randTimer;
    srand((unsigned) time(&randTimer));
    return (rand() % 100);
}

void writeIntValueOnFile(int value)
{
    pthread_mutex_lock(&mutex_file);
    FILE * file = fopen(FILE_DATA_NAME, FILE_OPEN_FOR_WRITING);
	if (file != NULL) 
	{
		fprintf(file, "%d",  value);
    	fclose(file);
	}
	pthread_mutex_unlock(&mutex_file);
}

void writeFile(void * arg)
{
    while (TRUE)
    {
		int randomValue = getRandomValue();
		writeIntValueOnFile(randomValue);
    }
}

int readIntValueFromFile()
{
    int value = -1;
    
    pthread_mutex_lock(&mutex_file);
    FILE * file = fopen(FILE_DATA_NAME, FILE_OPEN_FOR_READING);
	if (file != NULL) 
	{
	    char buffer[16];
        fscanf(file, "%s", buffer);
        sscanf(buffer, "%d", &value);
    	fclose(file);
	}
	pthread_mutex_unlock(&mutex_file);
	
	return value;
}

bool isOnRange(int value)
{
    return (value >= MIN_RANGE_VALUE  && value <= MAX_RANGE_VALUE);
}

void readFile(void * arg)
{
    int oldValue = -1;
    while (TRUE)
    {
        int value = readIntValueFromFile();
        
        if (value == oldValue)
            continue;
        
        if (!isOnRange(value))
            continue;
        
        oldValue = value;    
        printf("%d\n",  value);
    }
}

void createThreadWriter()
{
    pthread_create(&threadWriter, NULL, &writeFile, NULL);
     
}

void createThreadReader()
{
    pthread_create(&threadReader, NULL, &readFile, NULL);
}

int main()
{
    createThreadWriter();
    createThreadReader();
    
    while (TRUE);
    return 0;
}

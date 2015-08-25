// Mikayla Ray & Mitchell Duncan
// 25 August 2015
// CS-451 : Operating Systems Concepts
// Producer-Consumer Problem
// Pthreads

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <lab3_def.h>

buffer_item buffer[BUFFER_SIZE];
int go = 1, rPos = 0, aPos = -1;
sem_t mutex, empty, full;

int insert(buffer_item item);
int remove(buffer_item *item);
void *consumer(void *param);
void *producer(void *param);

int main(int argc, char*argv[]) {
   int sleepTime = atoi(argv[1]);
   int numProduce = atoi(argv[2]);
   int numConsume = atoi(argv[3]);
   
   int i = 0;
   int j = 0;
   
   pthread_t prothread[NUM_THREADS];
   pthread_t conthread[NUM_THREADS];
   
   sem_init(&mutex,0,1);
   sem_init(&empty,0,BUFFER_SIZE);
   sem_init(&full,0,0);
   
   printf("Pthreads used:\n");
   
   if(NUM_THREADS < numProduce) {
      printf("The number of producer threads requested exceeds capacity.");
      return 1;
   }
   
   if(NUM_THREADS < numConsume) {
      printf("The number of consumer threads requested exceeds capacity.");
      return 2;
   }
   
   printf("Sleep: %d\tProducer Threads: %d\tConsumer Threads: %d\n\n",sleepTime,numProduce,numConsume); 
   
   for(i = 0; i < numProduce; i++)
      if(pthread_create(&prothread[i], NULL, producer, (void*)i))
         printf("ERROR");
      
   for(j = 0; j < numConsume; j++)
      pthread_create(&conthread[j], NULL, consumer, (void*)(j+i));
   
   sleep(sleepTime);
   go = 0;
   pthread_exit(NULL);
   return 0;
}

int insert(buffer_item item) {
   sem_wait(&mutex);
   sem_wait(&empty);
   
   aPos++;
   
   if(buffer[aPos%BUFFER_SIZE] != NULL) {
      printf("The buffer is full!\n");
      sem_post(&full);
      sem_post(&mutex);
      return -1;
   }
   
   buffer[aPos%BUFFER_SIZE] = item;
   sem_post(&full);
   sem_post(&mutex);
   return 0;
}

int remove(buffer_item *item) {
   sem_wait(&full);
   sem_wait(&mutex);
   
   if(rPos < 0 || buffer[rPos%BUFFER_SIZE] == NULL) {
      printf("Cannot remove from an empty buffer!\n");
      rPos++;
      sem_post(&mutex);
      sem_post(&empty);
      return -1;
   }  
   buffer[rPos%BUFFER_SIZE] = NULL;   
   rPos++;
   sem_post(&mutex);
   sem_post(&empty);
   return 0;   
}

void *producer(void *param) {
   buffer_item item;
   
   while(go) {
      sleep(2);
      item = rand() %100+1;
      
      if(insert(item))
         printf("ERROR");
      else
         printf("Thread #%d produced %d at position: %d\n", param, item, aPos%BUFFER_SIZE);
   }
}

void *consumer(void *param) {   
   int index;
   while(go) {
      sleep(2);
      index = rPos%BUFFER_SIZE;
      
      if(remove(rPos))
         printf("ERROR AT: %d\n",index);
      else
         printf("Thread #%d consumed position: %d\n",param,index);
   }
}
/*
  Sarah Moore
  slmoore1@mix.wvu.edu
  800099213
  Oliver Wiegand
  owwiegand@mix.wvu.edu
  800098942
 */


#ifndef WVU_THREAD_POOL_H_
#define WVU_THREAD_POOL_H_

#include "job.h"
#include "queue.h"
#include <pthread.h>

#define TRUE 1
#define FALSE 0

typedef struct ThreadPool{
  int workers;
  int active;
  Queue* queue;
  pthread_t* threads;
  pthread_attr_t joinable;
  pthread_mutex_t* mutex_ptr;
} ThreadPool;

ThreadPool* CreateThreadPool(int);
void DestroyThreadPool(ThreadPool*);
void EnqueueJob(ThreadPool*, Job*);
void ExecuteJobs(ThreadPool*);
void* PerformJob(void*);
int IsThreadPoolActive(ThreadPool*);
int GetNumberOfRemainingJobs(ThreadPool*);


#endif

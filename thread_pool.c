/*
  Sarah Moore
  slmoore1@mix.wvu.edu
  800099213
  Oliver Wiegand
  owwiegand@mix.wvu.edu
  800098942
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread_pool.h"

//Malloc space for the ThreadPool Struct
ThreadPool* CreateThreadPool(int numberOfWorkers){
  //If there are no workers or if there is a negative amount of workers, program returns NULL
  if(numberOfWorkers <= 0){
    return NULL;
  }
  //Malloc space for the ThreadPool
  ThreadPool* thread_pool = (ThreadPool*)malloc(sizeof( ThreadPool));
  //Cheeck to make sure that the threadpool was malloced correctly
  //A NULL will be returned if it wasn't
  if(!thread_pool){
    return NULL;
  }
  //Sets the number of workers of the thread pool equal to the inputted integer
  thread_pool->workers = numberOfWorkers;
  //Sets the thread pool's active status to false
  thread_pool->active = FALSE;
  //Sets the pointer to the job queue to null
  thread_pool->queue = CreateQueue();
  if(!thread_pool->queue){
    return NULL;
  }
  //creates all of the threads for the thread pool
  thread_pool->threads = (pthread_t*)malloc(sizeof(pthread_t)*numberOfWorkers);
  //Cheaks to make sure that the array of threads was created properly
  //A NULL will be returned if it wasn't
  if(!thread_pool->threads){
    return NULL;
  }
  return thread_pool;
}


//If the job queus is NULL for the thread pool, create it and add the job
//Otherwise, simply add the job
void EnqueueJob(ThreadPool* thread_pool, Job* job){
  if(!thread_pool){
    return;
  }
  if(!job){
    return;
  }
  Enqueue(thread_pool->queue, job);
}


void* PerformJob(void* argument){
  if(!argument){
    return NULL;
  }
  ThreadPool* input = (ThreadPool*)argument;
  if(!input->queue){
    return NULL;
  }

  while(GetQueueSize(input->queue) > 0){
    pthread_mutex_lock(input->mutex_ptr);
    Job* job = Dequeue(input->queue);
    pthread_mutex_unlock(input->mutex_ptr);
    job->job_description(job->input, job->output);
  }
 
 //active = FALSE;
  pthread_exit(NULL);
}

//Grab jobs from the queue and perform compute_centroid
void ExecuteJobs(ThreadPool* thread_pool){
  //Verifies that the thread pool is not NULL
  if(!thread_pool){
    return;
  }

  //Sets the active status to true
  thread_pool->active = TRUE;
  //Initializes the mutex
  pthread_mutex_t mutex;
 //Initialized the joinable attribute
  pthread_attr_init(&thread_pool->joinable);
  pthread_attr_setdetachstate(&thread_pool->joinable, PTHREAD_CREATE_JOINABLE);
  pthread_mutex_init(&mutex, NULL);
  thread_pool->mutex_ptr = &mutex;

  //Initialize counters
  int worker_counter = thread_pool->workers;
  int i = 0, j = 0;

  for(i = 0; i < worker_counter; ++i){
    if(pthread_create(&thread_pool->threads[i], &thread_pool->joinable, &PerformJob, thread_pool)){
      return;
    }
    sleep(1);
  }

  for(j = 0; i < worker_counter; ++j){
    pthread_join(thread_pool->threads[j], NULL);
  }

}



//Returns a boolean corresponding to the Thread Pool's active status
int IsThreadPoolActive(ThreadPool* thread_pool){
  //Verifies that the thread pool isn't null
  //If the thread pool is null it cannot be active
  if(!thread_pool){
    return FALSE;
  }
  //Returns the boolean value corresponding to the thread pool's active status
  return thread_pool->active;
}

//Returns an int value corresponding to the size of the job queue
int GetNumberOfRemainingJobs(ThreadPool* thread_pool){
  //If the thread pool is NULL, -1 is returned
  if(!thread_pool){
    return -1;
  }
  //If the Job Queue corresponding to the thread pool is NULL, -1 is returned.
  if(!thread_pool->queue){
    return -1;
  }
  //The size of the job queue corresponding to the thread pool is returned
  return thread_pool->queue->size;
}

//Deallocated Memory given to the Thread Pool
void DestroyThreadPool(ThreadPool* thread_pool){
  //If the thread pool is null, there is nothing to deallocate
  if(!thread_pool){
    return;
  }
  //destroys the joinable attribute for each thred
  pthread_attr_destroy(&thread_pool->joinable);
  //Sets the active status to false
  thread_pool->active = FALSE;
  pthread_mutex_destroy(thread_pool->mutex_ptr);
  //Sets the sturcture equal to NULL
  thread_pool->workers = 0;
  thread_pool->queue = NULL;
  thread_pool = NULL;
  //frees the memory
  free(thread_pool);
}

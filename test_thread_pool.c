/* Final project for CS 350 - Spring 2017.
   Author: Victor Fragoso <victor.fragoso@mail.wvu.edu>

   Compilation:

   $ gcc test_thread_pool.c thread_pool.c queue.c job.c -o test_thread_pool -lpthread
 */

#include <stdio.h>
#include <stddef.h>  /* For NULL. */
#include <unistd.h>  /* For usleep. */
#include "thread_pool.h"

#define MAX_JOBS 64
#define MAX_WORKERS 1

/* Task description for each worker. */
void PrintIncreasedInput(void* input, void* output) {
  if (!input || !output) {
    return;
  }
  int* num_input = (int*) input;
  int output_number = *num_input;
  int* num_output = (int*) output;
  *num_output = ++output_number;
  printf("[Job] Input: %d Output: %d\n", *num_input, *num_output);
}

/* Input/output buffers for test. */
int input_numbers[MAX_JOBS];
int output_numbers[MAX_JOBS];

int main() {
  ThreadPool* thread_pool;
  Job* job;
  int i = 0;
  int num_errors = 0;
  int queue_size = 0;
  /* Test 1. Create a thread pool with an invalid number of workers. */
  if (CreateThreadPool(-1) || CreateThreadPool(0)) {
    puts("[ERROR] Thread pool does not handle invalid input arguments.");
    ++num_errors;
  }
  /* Test 2. Create a thread pool with MAX_WORKERS. */
  thread_pool = CreateThreadPool(MAX_WORKERS);
  if (!thread_pool) {
    puts("[ERROR] Thread pool does not create a valid thread pool.");
    ++num_errors;
  }
  /* Test 3. Enqueue jobs. */
  for (i = 0; i < MAX_JOBS; ++i) {
    input_numbers[i] = i;
    job = CreateJob(&PrintIncreasedInput, i,
                    input_numbers + i, output_numbers + i);
    EnqueueJob(thread_pool, job);
  }
  queue_size = GetNumberOfRemainingJobs(thread_pool);
  if (queue_size != MAX_JOBS) {
    puts("[ERROR] Thread pool did not enqueue jobs correctly.");
    printf("[ERROR] Queue size: %d\n", queue_size);
    ++num_errors;    
  }
  /* Test 4. Initialize a valid thread pool.*/
  ExecuteJobs(thread_pool);
  printf("%d\n", IsThreadPoolActive(thread_pool));
  if (!IsThreadPoolActive(thread_pool)) {
    puts("[ERROR] Thread pool is not initialized correctly.");
    ++num_errors;
  }

  printf("%d\n", GetNumberOfRemainingJobs(thread_pool));
  while (GetNumberOfRemainingJobs(thread_pool)) {
    printf("Waiting for jobs to be completed ...\n");
    /* Sleep for 1 ms = 1000 micro seconds.*/
    usleep(1000);
  }
  
  /* Test 5. Destroy thread pool. */
   queue_size = GetNumberOfRemainingJobs(thread_pool);
  if (queue_size != 0) {
    puts("[ERROR] Thread pool did not clear the jobs queue correctly.");
    printf("[ERROR] Queue size: %d\n", queue_size);
    ++num_errors;
  }

 DestroyThreadPool(thread_pool);
  if (thread_pool->workers || thread_pool->queue) {
    puts("[ERROR] Thread pool did not set pointers to NULL after deletion.");
    ++num_errors;    
  }
  if (num_errors) {
    printf("Test finish with %d errors.\n", num_errors);
  } else {
    puts("All tests passed!");
  }
  return 0;
}

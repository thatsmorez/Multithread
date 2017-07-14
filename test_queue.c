/* Final project for CS 350 - Spring 2017.
   Author: Victor Fragoso <victor.fragoso@mail.wvu.edu>

   Compilation:

   $ gcc test_queue.c queue.c job.c -o test_queue
 */

#include <stdio.h>

#include "job.h"
#include "queue.h"

int VerifyQueue(const Queue* queue, const int max_elements) {
  if (!queue) {
    return -1;
  }
  Job* current = queue->first;
  if (!queue->first) {
    printf("Empty queue!");
    return 1;
  }
  int i = 0;
  int num_errors = 0;
  while (current->next) {
    if (i != current->job_id) {
      printf("Wrong enqueing: Expected job id: %d read id: %d\n",
             i, current->job_id);
      ++num_errors;
    }
    current = current->next;
    ++i;
  }
  return num_errors;
}

int main() {
  Queue* queue = CreateQueue();
  int i;
  Job* job;
  const int kMaxElements = 128;
  int num_errors = 0;

  /* Test 1. Attempt to dequeue element from an empty queue. */
  if (Dequeue(queue)) {
    puts("[ERROR]: Expected null pointer.");
    ++num_errors;
  }

  /* Test 2. Insert elements into the queue. */
  for (i = 0; i < kMaxElements; ++i) {
    job = CreateJob(NULL, i, NULL, NULL);
    Enqueue(queue, job);
    /* Check that the size increases. */
    if (GetQueueSize(queue) != (i + 1)) {
      puts("[ERROR]: Queue does not increase size correctly!");
      ++num_errors;
    }
  }

  if (VerifyQueue(queue, kMaxElements)) {
    puts("[ERROR]: Queue is not build correctly!");
    ++num_errors;
  }

  /* Test 3. Dequeue elements. */
  i = 0;
  while (queue->size) {
    job = Dequeue(queue);
    if (job->job_id != i) {
      printf("[ERROR] Wrong retrieval of job: %d. Returned job %d\n", i, job->job_id);
    }
    DestroyJob(job);
    ++i;
  }

  /* Test 4. Enqueue with invalid input. If not handled correctly, the program
     will crash very likely. */
  Enqueue(queue, NULL);
  Enqueue(NULL, NULL);

  /* Test 5. Destroy queue. The queue owns the jobs, which means that every job
   in the queue must deallocate the elements individually. */
  job = CreateJob(NULL, i, NULL, NULL);
  Enqueue(queue, job);
  DestroyQueue(queue);
  if (GetQueueSize(queue) != 0 || queue->first != NULL) {
    puts("[ERROR]: Bad deallocation of queue!");
    ++num_errors;
  }
  /* Test 6. Destroy an invalid queue. */
  DestroyQueue(NULL);
  /* Test 7. Get size of invalid queue. */
  if (GetQueueSize(NULL) > -1) {
    puts("[ERROR]: Bad  size of queue!");
    ++num_errors;
  }
  printf("Number of errors: %d\n", num_errors);
  if (!num_errors) {
    puts("All tests passed!");
  }
  return 0;
}

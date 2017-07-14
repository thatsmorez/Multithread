#include "victor_tests/job.h"
#include "queue.h"
#include <stdio.h>


int main(){
  Queue* MyQueue = CreateQueue();
  if(!MyQueue){
    printf("Queue Was NULL! \n");
  }

  if(!MyQueue->first){
    printf("First Element Was NULL! \n");
  }

  if(!MyQueue->last){
    printf("Last Element Was NULL! \n");
  }
  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));


  Job* job = CreateJob(NULL, 1, NULL, NULL);
  Job* job1 = CreateJob(NULL, 2, NULL, NULL);
  Job* job2 = CreateJob(NULL, 3, NULL, NULL);
  Job* job3 = CreateJob(NULL, 4, NULL, NULL);
  Job* job4 = CreateJob(NULL, 5, NULL, NULL);
  Job* job5 = CreateJob(NULL, 6, NULL, NULL);

  Enqueue(MyQueue, job);
  Enqueue(MyQueue, job1);
  Enqueue(MyQueue, job2);
  Enqueue(MyQueue, job3);
  Enqueue(MyQueue, job4);
  Enqueue(MyQueue, job5);

  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));

  Job* job6 = Dequeue(MyQueue);
  printf("Job ID: %d \n", job6->job_id);
  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));

  Job* job7 = Dequeue(MyQueue);
  printf("Job ID: %d \n", job7->job_id);
  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));

  Job* job8 = Dequeue(MyQueue);
  printf("Job ID: %d \n", job8->job_id);
  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));

  Job* job9 = Dequeue(MyQueue);
  printf("Job ID: %d \n", job9->job_id);
  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));


  Job* job10 = Dequeue(MyQueue);
  printf("Job ID: %d \n", job10->job_id);
  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));


  Job* job11 = Dequeue(MyQueue);
  printf("Job ID: %d \n", job11->job_id);
  printf("Num of Elements: %d \n", GetQueueSize(MyQueue));

  Enqueue(MyQueue, job5);

  printf("Before Destroy Num of Elements: %d \n", GetQueueSize(MyQueue));

  DestroyQueue(MyQueue);

  printf("After Destroy Num of Elements: %d \n", GetQueueSize(MyQueue));

  return 0;
}

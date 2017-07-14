/*
  Sarah Moore
  slmoore1@mix.wvu.edu
  800099213
  Oliver Wiegand
  owwiegand@mix.wvu.edu
  800098942
 */

#include <stdlib.h>
#include "job.h"
#include "queue.h"

/*
Malloc space for the Queue Struct
*/
Queue* CreateQueue(){
  //Malloc space for the queue.
  Queue* new_queue = (Queue*) malloc(sizeof(Queue));

  //Check to make sure that the queue was malloced correctly.
  //A NULL will be returned to the user if it wasnt.
  if(!new_queue){
    return NULL;
  }
  //Set the pointers to the first and last elements to NULL.
  new_queue->first = NULL;
  new_queue->last = NULL;

  //Set the Queue Size to 0
  new_queue->size = 0;

  //Return the address of the queue.
  return new_queue;
}

/*
2) Insert the job at the end of the queue.
3) Set the old end job.next = the new job
*/
void Enqueue(Queue* Queue, Job* job_to_add){
  //Check to make sure that the user didnt pass in NULL pointers.
  if(!Queue || !job_to_add){
    return;
  }

  //Update the Queue Structure
  if(Queue->first == NULL && Queue->last == NULL){
    //There isn't any elements current in the Queue. We are adding our first element
    Queue->first = job_to_add;
    Queue->last = job_to_add;
  } else {
    //There are already elements in the queue
    //Only update the last element and the conneection between the prev last element
    (Queue->last)->next = job_to_add;
    Queue->last = job_to_add;

  }

  //Increase The Current Size by 1
  Queue->last->next = NULL;
  Queue->size = Queue->size + 1;
}

/*
1) Return the job that is pointed to by Queue's first member*
2) Update Queue's first member to point to job.next
*/
Job* Dequeue(Queue* our_queue){
  //Check to make sure that user doesnt pass in a NULL pointer.
  if(!our_queue){
    return NULL;
  }

  //Get the pointer to the first element in the queue job
  Job* job_to_dequeue = our_queue->first;

  //There isn't any job there. return NULL
  if(!job_to_dequeue){
    return NULL;
  }

  //Update the Queue Structure
  //If the first_element and last_element point to the same
  //address we only have 1 element in the queue
  if(our_queue->first == our_queue->last){
    //We only have 1 element in the last
    our_queue->first = NULL;
    our_queue->last = NULL;

    //Set the queue size to 0
    our_queue->size = 0;
  } else {
    //We have more than 1 element in the list
    //Set the next element as the first element in the list
    our_queue->first= job_to_dequeue->next;

    //Decrease the queue size by 1
    our_queue->size = our_queue->size - 1;
  }

  return job_to_dequeue;

}
/*
Return the size of the Queue
*/
int GetQueueSize(Queue* our_queue){
  //Check to makre sure that the user doesnt pass in a NULL pointer.
  if(!our_queue){
    return -1;
  }
  //Return the size of the Queue
  return our_queue->size;
}

/*
Destroy the Queue
*/
void DestroyQueue(Queue* our_queue){
  if(!our_queue){
   return;
  }

 while(!Dequeue(our_queue)){
   Job* current_element = Dequeue(our_queue);
   DestroyJob(current_element);
 }

}

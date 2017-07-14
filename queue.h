/*
  Sarah Moore
  slmoore1@mix.wvu.edu
  800099213
  Oliver Wiegand
  owwiegand@mix.wvu.edu
  800098942
 */

/* Functions needed for the queue:
CreateQueue();
Dequeue();
Enqueue();
GetQueueSize();
DestroyQueue();
*/

#ifndef WVU_QUEUE_H_
#define WVU_QUEUE_H_

/* Queue Structure Struct */
typedef struct Queue{
    struct Job* first;
    struct Job* last;
    int size;
} Queue;

Queue* CreateQueue();
Job* Dequeue(Queue*);
void Enqueue(Queue*, Job*);
int GetQueueSize(Queue*);
void DestroyQueue(Queue*);


#endif

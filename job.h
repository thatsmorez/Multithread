/* Final project for CS 350 - Spring 2017.
   Author: Victor Fragoso <victor.fragoso@mail.wvu.edu>
 */

#ifndef JOB_H_
#define JOB_H_

/* Structure defining the job for the workers in the thread pool. */
typedef struct Job {
  /* Pointer to a function describing the task to execute. */
  void (*job_description)(void* input, void* output);
  /* Member to build a queue. */
  struct Job* next;
  /* The job id. */
  int job_id;
  /* Pointer to the input to the thread. */
  void* input;
  /* Pointer to the output of the thread. */
  void* output;
} Job;

/* Constructor for a job. The constructor allocates memory in the heap. The
   constructor returns NULL when not successfull, and a valid pointer in the
   heap otherwise.
   Params:
     job_description  A pointer to a function describing the task to execute.
     job_id  The job id.
     input  A pointer to the input "object" containing input data.
     output  A pointer to the output "object".

   It is safe to pass input and output as NULL. This is because there may be
   functions that do not require input or output.
 */
Job* CreateJob(void (*job_description)(void* input, void* output),
               int job_id, void* input, void* output);

/* Destructor for the job. */
void DestroyJob(Job* job);

#endif

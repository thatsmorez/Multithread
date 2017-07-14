/* Final project for CS 350 - Spring 2017.
   Author: Victor Fragoso <victor.fragoso@mail.wvu.edu>
 */

#include "job.h"
#include <stdlib.h>  /* For malloc and free. */

Job* CreateJob(void (*job_description)(void* input, void* output),
               int job_id, void* input, void* output) {
  Job* job = (Job*) malloc(sizeof(Job));
  if (!job) {
    return NULL;
  }
  job->job_description = job_description;
  job->job_id = job_id;
  job->input = input;
  job->output = output;
  return job;
}

void DestroyJob(Job* job) {
  if (job) {
    free(job);
  }
}

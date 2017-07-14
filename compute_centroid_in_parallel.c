/* Final project for CS 350 - Spring 2017.
   Author: Victor Fragoso <victor.fragoso@mail.wvu.edu>

   Compilation:

   $ gcc compute_centroid_in_parallel.c queue.c thread_pool.c job.c -o compute_centroid_in_parallel -lpthread
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>  /* For NULL. */
#include <unistd.h>  /* For usleep. */
#include "thread_pool.h"

#define MAX_FILES 32

typedef struct Point {
  float x;
  float y;
  float z;
} Point;

typedef struct AccumulatedPoint {
  Point point;
  int num_accumulated_points;
} AccumulatedPoint;

void AccumulatePoints(void* input, void* output) {
  if (!input || !output) {
    return;
  }
  char* input_filename = (char*) input;
  AccumulatedPoint* accumulated_point = (AccumulatedPoint*) output;
  /* Read points from file. */
  FILE* file = fopen(input_filename, "rb");
  int num_points;
  Point* points;
  int i;
  if (!file) {
    fprintf(stderr, "Could not open file: %s\n", input_filename);
    return;
  }
  fread(&num_points, sizeof(num_points), 1, file);
  points = (Point*) calloc(num_points, sizeof(Point));
  if (!points) {
    fprintf(stderr, "Could not allocate memory for points\n");
    return;
  }
  fread(points, sizeof(Point), num_points, file);
  fclose(file);
  /* Accumulate points. */
  accumulated_point->point.x = 0.0f;
  accumulated_point->point.y = 0.0f;
  accumulated_point->point.z = 0.0f;
  for (i = 0; i < num_points; ++i) {
    accumulated_point->point.x += points[i].x;
    accumulated_point->point.y += points[i].y;
    accumulated_point->point.z += points[i].z;
  }
  accumulated_point->point.x /= num_points;
  accumulated_point->point.y /= num_points;
  accumulated_point->point.z /= num_points;
  accumulated_point->num_accumulated_points = num_points;
  free(points);
  points = NULL;
}

void RemoveNewLine(char* filename) {
  while (*filename != '\0') {
    if (*filename =='\n') {
      *filename = '\0';
      return;
    }
    ++filename;
  }
}

int main(int argc, char** argv) {
  ThreadPool* thread_pool;
  Job* job;
  int i = 0;
  int queue_size = 0;
  int num_workers;
  char line[512];
  FILE* file;
  char* filenames[MAX_FILES] = {0};
  AccumulatedPoint acc_points[MAX_FILES] = {0};
  char* filename_ptr;
  AccumulatedPoint* acc_point_ptr;
  int num_lines = 0;
  AccumulatedPoint centroid;
  if (argc != 3) {
    printf("Usage: %s <file containing list of point-files> <num. of threads>\n",
           argv[0]);
    return -1;
  }
  
  /* Create a thread pool. */
  num_workers = atoi(argv[2]);
  if (num_workers < 1) {
    fprintf(stderr, "Cannot create a thread pool with %d workers\n",
            num_workers);
    return -1;
  }
  thread_pool = CreateThreadPool(num_workers);
  if (!thread_pool) {
    puts("[ERROR] Thread pool does not create a valid thread pool.");
    return -1;
  }
  /* Create jobs. */
  file = fopen(argv[1], "r");
  if (!file) {
    printf("Cannot open file %s\n", argv[1]);
    return -1;
  }
  for (i = 0; i < MAX_FILES && fgets(line, 512, file); ++i) {
    filename_ptr = (char*) malloc(strlen(line) + 1);
    if (!filename_ptr) {
      printf("Could not allocate memory for filename!\n");
      return -1;
    }
    strcpy(filename_ptr, line);
    RemoveNewLine(filename_ptr);
    acc_point_ptr = acc_points + i;
    if (!acc_point_ptr) {
      printf("Could not allocate memory for accumulated point!\n");
      return -1;      
    }
    printf("Creating job for file %s\n", filename_ptr);
    job = CreateJob(&AccumulatePoints, i, filename_ptr, acc_point_ptr);
    EnqueueJob(thread_pool, job);
  }
  fclose(file);
  num_lines = i;
  /* Launch jobs. */
  printf("Launching jobs ...\n");
  ExecuteJobs(thread_pool);
  while (GetNumberOfRemainingJobs(thread_pool)) {
    printf("Waiting for jobs to be completed ...\n");
    /* Sleep for 1 ms = 1000 micro seconds.*/
    usleep(1000);
  }
  /* Compute centroid and deallocate resources. */
  printf("Computing centroid ...\n");
  centroid.point.x = 0.0f;
  centroid.point.y = 0.0f;
  centroid.point.z = 0.0f;
  centroid.num_accumulated_points = 0;
  for (i = 0; i < num_lines; ++i) {
    if (filenames[i]) {
      free(filenames[i]);
    }
    centroid.point.x += acc_points[i].point.x;
    centroid.point.y += acc_points[i].point.y;
    centroid.point.z += acc_points[i].point.z;
    centroid.num_accumulated_points += acc_points[i].num_accumulated_points;
  }
  centroid.point.x /= centroid.num_accumulated_points;
  centroid.point.y /= centroid.num_accumulated_points;
  centroid.point.z /= centroid.num_accumulated_points;
  printf("Centroid: x=%f y=%f z=%f\n",
         centroid.point.x, centroid.point.y, centroid.point.z);
  DestroyThreadPool(thread_pool);
  return 0;
}

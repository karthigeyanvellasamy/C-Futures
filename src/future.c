#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// Future structure
typedef struct {
    void *result;               // Pointer to store the result
    bool is_complete;           // Flag indicating whether the task is complete
    pthread_mutex_t lock;       // Mutex for thread-safe access
    pthread_cond_t cond;        // Condition variable for signaling
} Future;

// Task structure
typedef struct {
    Future *future;             // Associated future
    void *(*task)(void *);      // Task function
    void *arg;                  // Argument for the task function
} Task;

// Worker function to execute a task
void *worker(void *arg) {
    Task *task = (Task *)arg;

    // Execute the task
    void *result = task->task(task->arg);

    // Store the result in the future
    pthread_mutex_lock(&task->future->lock);
    task->future->result = result;
    task->future->is_complete = true;
    pthread_cond_signal(&task->future->cond);
    pthread_mutex_unlock(&task->future->lock);

    free(task); // Free the task structure
    return NULL;
}

// Function to submit a task
Future *submit_task(void *(*task)(void *), void *arg) {
    // Initialize the future
    Future *future = (Future *)malloc(sizeof(Future));
    future->result = NULL;
    future->is_complete = false;
    pthread_mutex_init(&future->lock, NULL);
    pthread_cond_init(&future->cond, NULL);

    // Create a task structure
    Task *task_struct = (Task *)malloc(sizeof(Task));
    task_struct->future = future;
    task_struct->task = task;
    task_struct->arg = arg;

    // Create a thread to execute the task
    pthread_t thread;
    pthread_create(&thread, NULL, worker, task_struct);
    pthread_detach(thread);

    return future;
}

// Function to get the result of a future
void *get_result(Future *future) {
    pthread_mutex_lock(&future->lock);
    while (!future->is_complete) {
        pthread_cond_wait(&future->cond, &future->lock);
    }
    void *result = future->result;
    pthread_mutex_unlock(&future->lock);
    return result;
}

// Cleanup function for a future
void cleanup_future(Future *future) {
    pthread_mutex_destroy(&future->lock);
    pthread_cond_destroy(&future->cond);
    free(future);
}

// Example task function
void *example_task(void *arg) {
    int *num = (int *)arg;
    int *result = (int *)malloc(sizeof(int));
    *result = (*num) * 2; // Simple computation
    return result;
}

int main() {
    int num = 10;

    // Submit a task
    Future *future = submit_task(example_task, &num);

    // Get the result
    int *result = (int *)get_result(future);
    printf("Result: %d\n", *result);

    // Cleanup
    free(result);
    cleanup_future(future);

    return 0;
}
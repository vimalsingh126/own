
A message queue is a common mechanism used for inter-thread communication in concurrent programming. It allows threads to exchange messages or data in a synchronized and orderly manner. Each thread can send messages to the queue, and other threads can receive those messages from the queue.

Here's an example of implementing a simple message queue using threads in C:

c
Copy code
#include <stdio.h>
#include <pthread.h>

#define MAX_MESSAGES 10

typedef struct {
    int messages[MAX_MESSAGES];
    int count;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} MessageQueue;

void init_queue(MessageQueue* queue) {
    queue->count = 0;
    queue->front = 0;
    queue->rear = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_full, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
}

void send_message(MessageQueue* queue, int message) {
    pthread_mutex_lock(&queue->mutex);

    // Wait until the queue is not full
    while (queue->count == MAX_MESSAGES) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }

    // Add the message to the queue
    queue->messages[queue->rear] = message;
    queue->rear = (queue->rear + 1) % MAX_MESSAGES;
    queue->count++;

    // Signal that the queue is not empty
    pthread_cond_signal(&queue->not_empty);

    pthread_mutex_unlock(&queue->mutex);
}

int receive_message(MessageQueue* queue) {
    pthread_mutex_lock(&queue->mutex);

    // Wait until the queue is not empty
    while (queue->count == 0) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }

    // Retrieve the message from the queue
    int message = queue->messages[queue->front];
    queue->front = (queue->front + 1) % MAX_MESSAGES;
    queue->count--;

    // Signal that the queue is not full
    pthread_cond_signal(&queue->not_full);

    pthread_mutex_unlock(&queue->mutex);

    return message;
}

void* producer(void* arg) {
    MessageQueue* queue = (MessageQueue*)arg;

    for (int i = 0; i < 20; i++) {
        send_message(queue, i);
        printf("Produced message: %d\n", i);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    MessageQueue* queue = (MessageQueue*)arg;

    for (int i = 0; i < 20; i++) {
        int message = receive_message(queue);
        printf("Consumed message: %d\n", message);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;
    MessageQueue queue;

    init_queue(&queue);

    pthread_create(&producer_thread, NULL, producer, (void*)&queue);
    pthread_create(&consumer_thread, NULL, consumer, (void*)&queue);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}
In this example, we define a MessageQueue structure that contains an array to hold the messages, along with some additional fields to manage the queue. The queue is protected by a mutex to ensure thread safety, and two condition variables are used to signal when the queue is not full (not_full) and when it is not empty (not_empty).

The send_message function is responsible for adding messages to the queue. It first locks

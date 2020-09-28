#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
    void* element;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* head;
    Node* tail;
    size_t size;
    size_t elementSize;
} Queue;

Queue* newQueue(size_t elementSize);
Queue* enqueue(Queue* queue, void* data);
void* dequeue(Queue* queue);
void* peek(Queue* queue);
void clear(Queue* queue);
void destroy(Queue* queue);

#endif /* QUEUE_H */
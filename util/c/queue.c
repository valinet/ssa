#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

/* Liniile comentate permit implementarea unei cozi care copiaza datele intr-un
   buffer nou alocat dinbamic */

Queue* newQueue(size_t elementSize) {
    Queue* queue;
    queue = malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->size = 0;
    queue->elementSize = elementSize;
    queue->head = NULL;
    return queue;
}

Queue* enqueue(Queue* queue, void* data) {
    Node* node;
    node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    /*node->element = malloc(queue->elementSize);
    if (node->element == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->element, data, queue->elementSize);*/
    node->element = data;
    if (queue->size == 0) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    return queue;
}

void* dequeue(Queue* queue) {
    Node* node;
    void* data = NULL;
    if (queue->size > 0) {
        node = queue->head;
        /*data = malloc(queue->elementSize);
        if (data == NULL) {
            return NULL;
        }
        memcpy(data, node->element, queue->elementSize);*/
        data = node->element;
        if (queue->size > 1) {
            queue->head = queue->head->next;
        } else {
            queue->head = NULL;
            queue->tail = NULL;
        }
        queue->size--;
        /*free(node->element);*/
        free(node);
    }
    return data;
}

void* peek(Queue* queue) {
    Node* node;
    void* data = NULL;
    if (queue->size > 0) {
        node = queue->head;
        /*data = malloc(queue->elementSize);
        if (data == NULL) {
            return NULL;
        }
        memcpy(data, node->element, queue->elementSize);*/
        data = node->element;
    }
    return data;
}

void clear(Queue* queue) {
    void* data;
    do {
        data = dequeue(queue);
        /*free(data);*/
    }
    while (data != NULL);
    queue->size = 0;
}

void destroy(Queue* queue) {
    clear(queue);
    free(queue);
}
#include "dequeue.h"

byte_ptr malloc_binary_data() {
    return (byte_ptr)malloc(sizeof(byte) * CHUNK_DATA_SIZE);
}

chunk_ptr malloc_chunk() {
    return (chunk_t *)malloc(sizeof(chunk_t));
}

chunk_ptr create_chunk(byte_ptr data, data_size_t size) {
    if (size > CHUNK_DATA_SIZE) {
        printf("Error: Chunk size is larger than CHUNK_DATA_SIZE");
        return NULL;
    } else {
        chunk_ptr new_chunk = (chunk_t *)malloc(sizeof(chunk_t));
        new_chunk->data = data;
        new_chunk->size = size;
        new_chunk->next = NULL;
        new_chunk->prev = NULL;
        return new_chunk;
    }
}

void free_chunk(chunk_ptr chunk) {
    if (chunk != NULL) {
        free(chunk->data);
    }
    free(chunk);
}

dequeue_t create_dequeue() {
    dequeue_t head = (chunk_t **)malloc(sizeof(chunk_t *));
    *head = NULL;
    return head;
}

void connect_chunks(chunk_ptr chunk1, chunk_ptr chunk2) {
    chunk1->next = chunk2;
    chunk2->prev = chunk1;
}

void pushback_chunk(dequeue_t head, chunk_ptr new_chunk) {
    if (*head == NULL) {
        *head = new_chunk;
    }
    connect_chunks((*head)->prev, new_chunk);
    connect_chunks(new_chunk, *head);
}

chunk_ptr pop_chunk(dequeue_t head) {
    if (*head == NULL) {
        printf("Dequeue is empty");
        return NULL;
    } else if ((*head)->next == *head) {
        chunk_ptr popped_chunk = *head;
        *head = NULL;
        popped_chunk->next = NULL;
        popped_chunk->prev = NULL;
        return popped_chunk;
    } else {
        chunk_ptr popped_chunk = *head;
        (*head)->prev->next = (*head)->next;
        (*head)->next->prev = (*head)->prev;
        *head = (*head)->next;
        popped_chunk->next = NULL;
        popped_chunk->prev = NULL;
        return popped_chunk;
    }
}

void print_chunk(chunk_ptr chunk) {
    if (chunk == NULL) {
        printf("Dequeue is empty\r\n");
    } else {
        printf("%s\n", chunk->data);
        printf("%d\n", chunk->size);
    }
}

void traverse_dequeue(dequeue_t head) {
    chunk_ptr current_chunk = *head;
    if (current_chunk == NULL) {
        printf("Dequeue is empty\r\n");
    } 
    do {
        print_chunk(current_chunk);
        current_chunk = current_chunk->next;
    } while (current_chunk != *head);
}
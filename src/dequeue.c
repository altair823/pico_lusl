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

dequeue_t create_dequeue() {
    dequeue_t head = (chunk_t **)malloc(sizeof(chunk_t *));
    *head = (chunk_t *)malloc(sizeof(chunk_t));
    (*head)->data = NULL;
    (*head)->size = 0;
    (*head)->next = *head;
    (*head)->prev = *head;
    return head;
}

void connect_chunks(chunk_ptr chunk1, chunk_ptr chunk2) {
    chunk1->next = chunk2;
    chunk2->prev = chunk1;
}

int pushback_chunk(dequeue_t head, chunk_ptr new_chunk) {
    if (head) {
        printf("Error: head is NULL");
        return -1;
    }
    connect_chunks((*head)->prev, new_chunk);
    connect_chunks(new_chunk, *head);
    return 0;
}
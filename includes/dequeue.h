#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_DATA_SIZE 256
typedef unsigned int data_size_t;

typedef unsigned char byte;
typedef byte *byte_ptr;

/// @brief Binary chunk structure
/// @details This structure is used to store a chunk of data in the dequeue. 
/// The data is stored in a char* and the size of the chunk is stored in an int.
/// The dequeue is a doubly linked list, so each chunk has a pointer to the next and previous chunk.
typedef struct _chunk {
    byte_ptr data;
    struct _chunk *prev;
    struct _chunk *next;
    data_size_t size;
} chunk_t;
typedef chunk_t *chunk_ptr;
typedef chunk_t **dequeue_t;

/// @brief Malloc a new binary data. 
/// @details This function will malloc a new binary data and return a pointer to it. The size of the new binary data is always DATA_CHUNK_SIZE.
/// @return A pointer to the new binary data.
byte_ptr malloc_binary_data();

/// @brief Malloc a new chunk.
/// @details This function will malloc a new chunk and return a pointer to it.
/// @return A pointer to the new chunk.
chunk_ptr malloc_chunk();

/// @brief Create a new chunk from binary data.
/// @details This function will create a new chunk from binary data and return a pointer to it.
/// @param data The binary data to create the chunk from.
/// @param size The size of the binary data.
/// @return A pointer to the new chunk.
chunk_ptr create_chunk(byte_ptr data, data_size_t size);

/// @brief Create a new Dequeue.
/// @details This function will create a new dequeue and return a pointer to it.
/// @return A pointer to the new dequeue.
dequeue_t create_dequeue();

/// @brief Connect two chunks together.
/// @details This function will connect two chunks together.
/// @param chunk1 The first chunk to connect.
/// @param chunk2 The second chunk to connect.
void connect_chunks(chunk_ptr chunk1, chunk_ptr chunk2);

/// @brief Pushback a chunk to the end of the dequeue.
/// @details This function will pushback a new chunk to the end of the dequeue.
/// @param dequeue The dequeue to pushback the chunk to.
int pushback_chunk(dequeue_t head, chunk_ptr new_chunk);

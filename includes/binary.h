#pragma once

#include "sd_card.h"
#include "ff.h"

#define CHUNK_DATA_SIZE 128
typedef unsigned char byte;
typedef byte *byte_ptr;

/// @brief Byte data structure.
/// @details This structure is used to store byte data.
typedef struct _byte_data {
    byte_ptr data;
    unsigned int size;
} byte_data;

/// @brief Binary file reader structure.
/// @details This structure is used to read binary files.
typedef struct _binary_reader {
    unsigned char chunk_count;
    unsigned int remain_byte_counter;
} binary_reader;

/// @brief Create and initialize binary file reader.
/// @details This function creates and initializes binary file reader.
/// @param file_path Path to the binary file.
/// @return Pointer to the binary file reader.
binary_reader *create_reader(char *file_path, FIL *file);

/// @brief Initialize file.
/// @details This function initializes file.
/// @param file_path Path to the binary file.
/// @param file Pointer to the file.
/// @return 0 if success, -1 if error.
int init_file(char *file_path, FIL *file);

/// @brief Read wanted bytes from the binary file
/// @details This function reads wanted bytes from the binary file.
/// @param reader Pointer to the binary file reader.
/// @param byte_count Number of bytes to read.
/// @return Pointer to the read bytes.
byte_data *read_bytes(unsigned int byte_count, FIL *file);
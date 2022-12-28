#include <stdio.h>
#include <stdlib.h>
#include "binary.h"

binary_reader *create_reader(char *file_path, FIL *file) {
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\r\n");
        return NULL;
    }
    binary_reader *reader = (binary_reader *)malloc(sizeof(binary_reader));
    reader->chunk_count = 0;
    reader->remain_byte_counter = 0;
    FATFS fs;
    FRESULT result;
    result = f_mount(&fs, "0:", 1);
    if (result != FR_OK) {
        printf("ERROR: Could not mount filesystem (%d)\r\n", result);
        return NULL;
    }
    result = f_open(file, file_path, FA_READ);
    if (result != FR_OK) {
        printf("ERROR: Could not open file (%d)\r\n", result);
        return NULL;
    }
    return reader;
}
int init_file(char *file_path, FIL *file) {
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\r\n");
        return -1;
    }
    FATFS fs;
    FRESULT result;
    result = f_mount(&fs, "0:", 1);
    if (result != FR_OK) {
        printf("ERROR: Could not mount filesystem (%d)\r\n", result);
        return -1;
    }
    result = f_open(file, file_path, FA_READ);
    if (result != FR_OK) {
        printf("ERROR: Could not open file (%d)\r\n", result);
        return -1;
    }
    return 0;
}

byte_data *read_bytes(unsigned int byte_count, FIL *file) {
    byte_data *b_data = (byte_data *)malloc(sizeof(byte_data));
    b_data->data = (byte_ptr)malloc(sizeof(byte) * byte_count);
    UINT read_count = 0;
    FRESULT result = f_read(file, b_data->data, byte_count, &read_count);
    b_data->size = read_count;
    if (result != FR_OK) {
        printf("ERROR: Could not read from file (%d)\r\n", result);
        return NULL;
    } else if (read_count != byte_count) {
        printf("ERROR: Could not read all bytes from file\r\n");
        return b_data;
    } else if (read_count == 0) {
        printf("EOF\r\n");
        return NULL;
    }
    return b_data;
}
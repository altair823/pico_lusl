#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "sd_card.h"
#include "ff.h"

#define FILE_LABEL "LUSL Serialized File"
#define FILE_VERSION_FLAG 0x01
#define MAJOR_VERSION 2
#define MINOR_VERSION 0
#define PATCH_VERSION 2

#define CHUNK_DATA_SIZE 32768
typedef unsigned char byte;
typedef byte *byte_ptr;

bool read_file(FIL *file, byte_ptr data, size_t label_len, UINT read_count) {
    if (f_read(file, data, label_len, &read_count) != FR_OK || read_count < label_len) {
        printf("ERROR: Could not read from file\r\n");
        return false;
    }
    return true;
}

int main() {
    char filename[] = "SPA_pico.srl";
    FIL file;

    // Initialize chosen serial port
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
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
    result = f_open(&file, filename, FA_READ);
    if (result != FR_OK) {
        printf("ERROR: Could not open file (%d)\r\n", result);
        return -1;
    }


    // Read file
    byte_ptr buf = NULL;
    UINT read_count = 0;

    // Check file label
    size_t label_len = strlen(FILE_LABEL);
    buf = (byte_ptr)malloc(sizeof(byte) * label_len);
    read_file(&file, buf, label_len, read_count);
    if (strncmp(FILE_LABEL, buf, label_len) != 0) {
        printf("ERROR: File is not a valid LUSL serialized file\r\n");
        return -1;
    }
    free(buf);

    // Check file version
    buf = (byte_ptr)malloc(sizeof(byte) * 4);
    read_file(&file, buf, 4, read_count);
    if (buf[0] != FILE_VERSION_FLAG) {
        printf("ERROR: File is not a valid LUSL serialized file\r\n");
        return -1;
    }
    if (buf[1] != MAJOR_VERSION || buf[2] > MINOR_VERSION || buf[3] > PATCH_VERSION) {
        printf("ERROR: File version is not compatible with this program\r\n");
        return -1;
    }
    free(buf);

    // Read flags
    buf = (byte_ptr)malloc(sizeof(byte) * 1);
    read_file(&file, buf, 1, read_count);
    if (buf[0] != 0) {
        printf("ERROR: Cannot read file with this program\r\n");
        return -1;
    }
    free(buf);

    // Read file count
    buf = (byte_ptr)malloc(sizeof(byte) * 1);
    read_file(&file, buf, 1, read_count);
    int file_count_bytes = buf[0];
    free(buf);
    buf = (byte_ptr)malloc(sizeof(byte) * file_count_bytes);
    read_file(&file, buf, file_count_bytes, read_count);
    int file_count = 0;
    // Convert byte array to int with little endian
    for (int i = 0; i < file_count_bytes; i++) {
        file_count += buf[i] << (i * 8);
    }
    free(buf);

    // Read files
    for (int i = 0; i < file_count; i++) {
        // Read file name
        buf = (byte_ptr)malloc(sizeof(byte) * 2);
        read_file(&file, buf, 2, read_count);
        int file_name_len = (buf[0] << 8) + buf[1];
        free(buf);
        buf = (byte_ptr)malloc(sizeof(byte) * file_name_len);
        read_file(&file, buf, file_name_len, read_count);
        char *path = (char *)malloc(sizeof(char) * (file_name_len + 1));
        strncpy(path, buf, file_name_len);
        path[file_name_len] = '\0';
        free(buf);

        // Copy directory in the file name
        int last_dir_index = 0;
        for (int i = file_name_len - 1; i >= 0; i--) {
            if (path[i] == '/') {
                last_dir_index = i;
                break;
            }
        }
        char *dir_name = (char *)malloc(sizeof(char) * (last_dir_index + 1));
        strncpy(dir_name, path, last_dir_index);
        dir_name[last_dir_index] = '\0';

        // Create directory
        int slash_index = 0;
        for (int i = 0; i < last_dir_index + 1; i++) {
            if (dir_name[i] == '/' || i == last_dir_index) {
                slash_index = i;
                char *sub_dir_name = (char *)malloc(sizeof(char) * (slash_index + 1));
                strncpy(sub_dir_name, dir_name, slash_index);
                sub_dir_name[slash_index] = '\0';
                result = f_mkdir(sub_dir_name);
                if (result != FR_OK && result != FR_EXIST) {
                    printf("ERROR: Could not create directory (%d)\r\n", result);
                    return -1;
                }
                free(sub_dir_name);
            }
        }
        if (result != FR_OK && result != FR_EXIST) {
            printf("ERROR: Could not create directory (%d)\r\n", result);
            return -1;
        }
        free(dir_name);

        // Read file type
        buf = (byte_ptr)malloc(sizeof(byte) * 1);
        read_file(&file, buf, 1, read_count);
        int file_type = buf[0];
        free(buf);

        // Read file size
        int size_bytes = file_type & 0xf;
        buf = (byte_ptr)malloc(sizeof(byte) * size_bytes);
        read_file(&file, buf, size_bytes, read_count);
        int file_size = 0;
        // Convert byte array to int with little endian
        for (int i = 0; i < size_bytes; i++) {
            file_size += buf[i] << (i * 8);
        }
        free(buf);

        // Read file checksum
        buf = (byte_ptr)malloc(sizeof(byte) * 32);
        read_file(&file, buf, 32, read_count);
        free(buf);

        printf("File name: %s\r\n", path);
        printf("File type: %d\r\n", file_type);
        printf("File size: %d\r\n", file_size);

        // Read file data with chunking and write to new file
        int chunk_count = file_size / CHUNK_DATA_SIZE;
        int last_chunk_size = file_size % CHUNK_DATA_SIZE;
        FIL result_file;
        f_open(&result_file, path, FA_WRITE | FA_CREATE_ALWAYS);
        int p = 0;
        for (int j = 0; j < chunk_count; j++, p++) {            
            buf = (byte_ptr)malloc(sizeof(byte) * CHUNK_DATA_SIZE);
            read_file(&file, buf, CHUNK_DATA_SIZE, read_count);
            UINT write_count = 0;
            f_write(&result_file, buf, CHUNK_DATA_SIZE, &write_count);
            free(buf);
            printf("%s Progress: %d%%\r", path, (p * 100) / chunk_count);
        }
        if (last_chunk_size > 0) {
            buf = (byte_ptr)malloc(sizeof(byte) * last_chunk_size);
            read_file(&file, buf, last_chunk_size, read_count);
            UINT write_count = 0;
            f_write(&result_file, buf, last_chunk_size, &write_count);
            free(buf);
        }
        f_close(&result_file);
        printf("%s Progress: 100%%\r\n", path);
        free(path);
    }
    printf("Total read count: %d\r\n", read_count);
    f_close(&file);
    // blink led
    while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
    
}
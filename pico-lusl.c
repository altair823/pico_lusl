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
#define PATCH_VERSION 1

#define CHUNK_DATA_SIZE 512
typedef unsigned char byte;
typedef byte *byte_ptr;

int main() {
    char filename[] = "bible.srl";
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
    // Wait for user to press 'enter' to continue
    // printf("\r\nSD card test. Press 'enter' to start.\r\n");
    // scanf("%s", &buf);
    // int size = strlen(buf);
    // filename = (char *)malloc(sizeof(char) * (size + 1));
    // strncpy(filename, buf + 1, size);


    // Read file
    byte_ptr data = NULL;
    UINT read_count = 0;

    // Check file label
    size_t label_len = strlen(FILE_LABEL);
    data = (byte_ptr)malloc(sizeof(byte) * label_len);
    if (f_read(&file, data, label_len, &read_count) != FR_OK || read_count < label_len) {
        printf("ERROR: Could not read from file (%d)\r\n", result);
        return -1;
    }
    if (strncmp(FILE_LABEL, data, label_len) != 0) {
        printf("ERROR: File is not a valid LUSL serialized file\r\n");
        return -1;
    }
    free(data);

    // Check file version
    data = (byte_ptr)malloc(sizeof(byte) * 4);
    if (f_read(&file, data, 4, &read_count) != FR_OK || read_count < 4) {
        printf("ERROR: Could not read from file (%d)\r\n", result);
        return -1;
    }
    if (data[0] != FILE_VERSION_FLAG) {
        printf("ERROR: File is not a valid LUSL serialized file\r\n");
        return -1;
    }
    if (data[1] != MAJOR_VERSION || data[2] > MINOR_VERSION || data[3] > PATCH_VERSION) {
        printf("ERROR: File version is not compatible with this program\r\n");
        return -1;
    }
    free(data);

    // Read file count
    data = (byte_ptr)malloc(sizeof(byte) * 1);
    if (f_read(&file, data, 1, &read_count) != FR_OK || read_count < 1) {
        printf("ERROR: Could not read from file (%d)\r\n", result);
        return -1;
    }

    do {
        data = (byte_ptr)malloc(sizeof(byte) * CHUNK_DATA_SIZE);
        FRESULT result = f_read(&file, data, CHUNK_DATA_SIZE, &read_count);
        if (result != FR_OK) {
            printf("ERROR: Could not read from file (%d)\r\n", result);
            break;
        }
        if (read_count > 0) {
            for (int i = 0; i < read_count; i++) {
                printf("%c", data[i]);
            }
        }
        free(data);
        //sleep_ms(200);
    } while(read_count > 0);
}
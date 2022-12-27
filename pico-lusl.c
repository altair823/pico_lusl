#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "sd_card.h"
#include "ff.h"

#include "dequeue.h"

int main() {

    FRESULT fr;
    FATFS fs;
    FIL fil;
    int ret;
    char buf[CHUNK_DATA_SIZE];

    // Initialize chosen serial port
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }

    // Wait for user to press 'enter' to continue
    // printf("\r\nSD card test. Press 'enter' to start.\r\n");
    // scanf("%s", &buf);
    // int size = strlen(buf);
    // filename = (char *)malloc(sizeof(char) * (size + 1));
    // strncpy(filename, buf + 1, size);
    char filename[] = "bible.txt";

    // Initialize SD card
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\r\n");
        return -1;
    }

    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("ERROR: Could not mount filesystem (%d)\r\n", fr);
        return -1;
    }

    // // Open file for writing ()
    // fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    // if (fr != FR_OK) {
    //     printf("ERROR: Could not open file (%d)\r\n", fr);
    //     return -1;
    // }

    // // Write something to file
    // ret = f_printf(&fil, "This is another test\r\n");
    // if (ret < 0) {
    //     printf("ERROR: Could not write to file (%d)\r\n", ret);
    //     f_close(&fil);
    //     return -1;
    // }
    // ret = f_printf(&fil, "of writing to an SD card.\r\n");
    // if (ret < 0) {
    //     printf("ERROR: Could not write to file (%d)\r\n", ret);
    //     f_close(&fil);
    //     return -1;
    // }

    // // Close file
    // fr = f_close(&fil);
    // if (fr != FR_OK) {
    //     printf("ERROR: Could not close file (%d)\r\n", fr);
    //     return -1;
    // }

    // Open file for reading
    fr = f_open(&fil, filename, FA_READ);
    if (fr != FR_OK) {
        printf("ERROR: Could not open file (%d)\r\n", fr);
        return -1;
    }

    // Print every line in file over serial
    printf("Reading from file '%s':\r\n", filename);
    printf("---\r\n");
    UINT read_count = 0;
    f_read(&fil, buf, sizeof(buf), &read_count);
    if (read_count == 0) {
        printf("EOF\r\n");
    } else {
        byte_ptr data = malloc_binary_data();
        memcpy(data, buf, read_count);
        printf("%s", &buf);
    }
    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        printf("ERROR: Could not close file (%d)\r\n", fr);
        return -1;
    }

    // Unmount drive
    f_unmount("0:");
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
    return 0;
}
// int main() {
//     stdio_init_all();
//     if (cyw43_arch_init()) {
//         printf("WiFi init failed");
//         return -1;
//     }
//     while (true) {
//         cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
//         sleep_ms(250);
//         cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
//         sleep_ms(250);
//     }
// }
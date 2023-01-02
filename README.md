# Deserializer of LUSL File For Raspberry Pi Pico

The C code that deserialize the [LUSL](https://github.com/altair823/LUSL) file.
The original LUSL codes has written by Rust and support compressing and encrypting. 

But when it serialize and deserialize files with no compressing and no encrypting, requires only limited performance. 
Raspberry Pi Pico can provide that kind of performance too!

With [SD card library](https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico), Pi Pico converts the file that in the SD card effectively. 

Deserializing is the only operation that supported currently because serializing in LUSL need to generate md5 checksums for all each files and there is no such method generating md5 in Pi Pico. 

## Feature

- Deserialize LUSL file. 


## Supported SD Card Formats

- FatFs supported file system

    - Fat32(tested)
    - exFat(tested)


## Wiring

Same as carlk3's [no-OS-FatFS-SD-SPI-RPi-Pico](https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico) readme. 

But if you using like [SZH-EKBZ-005](https://www.devicemart.co.kr/goods/view?no=1326951&NaPm=ct%3Dlcdtj3zb%7Cci%3Dcheckout%7Ctr%3Dppc%7Ctrx%3D%7Chk%3D404ed6ec94b96676269a734b038d7d17e1b34277) as I am, 
change power pin to 40 or other else 5v input. 

The CD(Card Detect) pin is optional if you use only a one card slot. 

|       | SPI0  | GPIO  | Pin   | SPI       | MicroSD 0 | Description            | 
| ----- | ----  | ----- | ---   | --------  | --------- | ---------------------- |
| MISO  | RX    | 16    | 21    | DO        | DO        | Master In, Slave Out   |
| CS0   | CSn   | 17    | 22    | SS or CS  | CS        | Slave (or Chip) Select |
| SCK   | SCK   | 18    | 24    | SCLK      | CLK       | SPI clock              |
| MOSI  | TX    | 19    | 25    | DI        | DI        | Master Out, Slave In   |
| CD    |       | 22    | 29    |           | CD        | Card Detect            |
| GND   |       |       | 18,23 |           | GND       | Ground                 |
| 3v3   |       |       | 36    |           | 3v3       | 3.3 volt power         |


## Warning

- If file path has non-ascii character such as utf-8, utf-16, etc., the function cannot creates directories and files normally. 
- Please be aware the limitation of Fat32 format when you using it(like 4GB file size limit).
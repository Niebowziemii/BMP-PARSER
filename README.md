# console BMP-Parser
### Authors 
##### Rafał Stachowiak - creator of the printable headers, histogram creation and grayscale image alghorytm in main.c and BMP_HEADER.h
##### Franciszek Olejnik - creator of the steganography part and Windows OS supportive functions covering I/O api in BMPParser.h

## Simplicity in usage
Few steps how to use our application:
1. compile file main.c 
```bash
gcc -o <executable-file-name> main.c
```
2.  execute with the following to obtain informations about headers, and histogram with percentage of colours:
```bash
./<executable-file-name> <file-name.bmp> 
```
3.  If you want to change your bmp file into grayscale (using mean colour method):
```bash
./<executable-file-name> <file-name.bmp> <result-file-name.bmp>
```
4. If you want to encrypt a message in the bmp file please execute as following:
```bash
./<executable-file-name> <file-name.bmp> <result-file-name.bmp> <"message-to-encrypt">
```
### BMP_HEADER.h 
It contains defines directives crucial for program execution and definitions for functions used in main.c to handle Headline printing, histogram creation and turning file into grayscale.
This library support executing file in Linux only

### BMPParser.h
It contains defines directives crucial for program execution and definitions for function to take out data from bmp file and to save it into new file. This library support Windows api.
This library support executing file in Linux and in Windows

#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifndef BFTYPE
#define BFTYPE *((uint16_t*)&buffer[0])
#endif // !BFTYPE
#ifndef BFSIZE
#define BFSIZE *((uint32_t*)&buffer[2])
#endif // !BFSIZE
#ifndef BFRESERVED1
#define BFRESERVED1 *((uint16_t*)&buffer[6])
#endif // !BFRESERVED1
#ifndef BFRESERVED2
#define BFRESERVED2 *((uint16_t*)&buffer[8])
#endif // !BFRESERVED2
#ifndef BFOFFBITS
#define BFOFFBITS *((uint32_t*)&buffer[10])
#endif // !BFOFFBITS
#ifndef BISIZE
#define BISIZE *((uint32_t*)&buffer[14])
#endif // !BISIZE
#ifndef BIWIDTH
#define BIWIDTH *((uint32_t*)&buffer[18])
#endif // !BIWIDTH
#ifndef BIHEIGHT
#define BIHEIGHT *((uint32_t*)&buffer[22])
#endif // !BIHEIGHT
#ifndef BIPLANES
#define BIPLANES *((uint16_t*)&buffer[26])
#endif// !BIPLANES
#ifndef BIBITCOUNT
#define BIBITCOUNT *((uint16_t*)&buffer[28])
#endif// !BIBITCOUNT
#ifndef BICOMPRESSION
#define BICOMPRESSION *((uint32_t*)&buffer[30])
#endif// !BICOMPRESSION
#ifndef BISIZEIMAGE
#define BISIZEIMAGE *((uint32_t*)&buffer[34])
#endif// !BISIZEIMAGE
#ifndef BIXPERLSPERMETER
#define BIXPERLSPERMETER *((uint32_t*)&buffer[38])
#endif// !BIXPERLSPERMETER
#ifndef BIYPERLSPERMETER
#define BIYPERLSPERMETER *((uint32_t*)&buffer[42])
#endif// !BIYPERLSPERMETER
#ifndef BICLRUSED
#define BICLRUSED *((uint32_t*)&buffer[46])
#endif// !BICLRUSED
#ifndef BICLRIMPORTANT
#define BICLRIMPORTANT *((uint32_t*)&buffer[50])
#endif// !BICLRIMPORTANT
#ifndef IF
#define IF(num) if(a >= num*16 && a<=(num+1)*16 - 1) { return num; }
#endif
void printData(char* buffer) {
    printf("BITMAPFILEHEADER: \nbfType: \t\t0x%X\t%d (BM)\nbfSize: \t\t0x%X\t%d\nbfReserved1: \t\t0x%X\t%d\nbfReserved2: \t\t0x%X\t%d\nbfOffBits: \t\t0x%X\t%d\n\n", BFTYPE, BFTYPE, BFSIZE, BFSIZE, BFRESERVED1, BFRESERVED1, BFRESERVED2, BFRESERVED2, BFOFFBITS, BFOFFBITS);
    printf("BITMAPINFOHEADER: \nbiSize: \t\t0x%X\t%d\nbiWidth: \t\t0x%X\t%d\nbiHeight: \t\t0x%X\t%d\nbiPlanes: \t\t0x%X\t%d\nbiBitCount: \t\t0x%X\t%d\nbiCompression: \t\t0x%X\t%d\nbiSizeImage: \t\t0x%X\t%d\nbiXPelsPerMeter: \t0x%X\t%d\nbiYPelsPerMeter: \t0x%X\t%d\nbiClrUsed: \t\t0x%X\t%d\nbiClrImportant: \t0x%X\t%d\n", BISIZE, BISIZE, BIWIDTH, BIWIDTH, BIHEIGHT, BIHEIGHT, BIPLANES, BIPLANES, BIBITCOUNT, BIBITCOUNT, BICOMPRESSION, BICOMPRESSION, BISIZEIMAGE, BISIZEIMAGE, BIXPERLSPERMETER, BIXPERLSPERMETER, BIYPERLSPERMETER, BIYPERLSPERMETER, BICLRUSED, BICLRUSED, BICLRIMPORTANT, BICLRIMPORTANT);
}
#ifdef linux
void strreverse(char* begin, char* end) {
    char aux;
    while (end > begin)
        aux = *end, * end-- = *begin, * begin++ = aux;
}
void itoa(int value, char* str, int base) {
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr = str;
    int sign;
    // Validate base
    if (base < 2 || base>35) { *wstr = '\0'; return; }
    // Take care of sign
    if ((sign = value) < 0) value = -value;
    // Conversion. Number is reversed.
    do *wstr++ = num[value % base]; while (value /= base);
    if (sign < 0) *wstr++ = '-';
    *wstr = '\0';
    // Reverse string
    strreverse(str, wstr - 1);
}
#endif
int checkNum(unsigned char* num) {
    int a = (int)*num;
    for (int i = 0; i < 16; i++) {
        IF(i);
    }
    return EXIT_FAILURE;
}
void printIt(int* counter, int sum, const char* mess) {
    printf("\n\n%s: \n\n", mess);
    for (int i = 0, j = 0; i < 16; j++, i++)
        printf("%d-%d: %.2lf%%\n", 16 * i, 16 * i + 15, (counter[j] / (double)sum) * 100);
}
void histogramate(char* buffer) {
    int offset = BFSIZE - BISIZEIMAGE;
    int padding = ((BIWIDTH * 3) % 4);
    int countBlue[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countGreen[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countRed[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // for every row
    for (int i = 0; i < BIHEIGHT; i++) {
        // for every pixel (triple)
        for (int j = 0; j < BIWIDTH * 3; j += 3) {
            unsigned char buffBlue;
            int b = 0;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (BIWIDTH * 3 + padding)), 1);
            b = checkNum(&buffBlue);
            countBlue[b]++;

            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (BIHEIGHT * 3 + padding) + 1), 1);
            b = checkNum(&buffGreen);
            countGreen[b]++;

            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (BIWIDTH * 3 + padding) + 2), 1);
            b = checkNum(&buffRed);
            countRed[b]++;
            //printf("Blue: %X Green: %X Red: %X\n", buffBlue, buffGreen, buffRed);
        }
        //printf("End of row. %d\n",i);
    }
    // obtain sum of all pixel values to check correctness 
    int sum = 0;
    for (int i = 0; i < 16; i++)
        sum += countBlue[i];
    // handle error
    if (BIHEIGHT * BIWIDTH != sum) {
        printf("Number of pixels in the histogram does noot match number of pixels in the image");
    }
    printIt(countBlue, sum, "Blue");
    printIt(countGreen, sum, "Green");
    printIt(countRed, sum, "Red");
}
void grayScaleMe(char* buffer, FILE * rfile) {
    // copy headers
    fwrite(buffer, 14 + BISIZE, 1, rfile);
    int offset = BFSIZE - BISIZEIMAGE;
    int padding = ((BIWIDTH * 3) % 4);
    // go through pixels
    for (int i = 0; i < BIHEIGHT; i++) {
        for (int j = 0; j < BIWIDTH * 3; j += 3) {
            unsigned char buffBlue;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (BIWIDTH * 3 + padding)), 1);
            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (BIWIDTH * 3 + padding) + 1), 1);
            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (BIWIDTH * 3 + padding) + 2), 1);
            int mean = ((int)buffBlue + (int)buffGreen + (int)buffRed) / 3;
            char g[3];
#ifdef linux
            itoa(mean, g, 16);
#endif 
#ifdef _WIN32
            _itoa(mean, g, 16);
#endif
            fwrite(g, 1, 1, rfile);
            fwrite(g, 1, 1, rfile);
            fwrite(g, 1, 1, rfile);
        }
    }
}
FILE* fileCreate(char** argv, int index, const char* mode) {
    FILE* file = NULL;
    file = fopen(argv[index], mode);
    if (!file) {
        printf("The file was not opened\n");
        return NULL;
    }
    return file;
}
char* allocateBuffer(uint32_t size) {
    char* buffer;
    buffer = (char*)malloc(size * sizeof(char));
    memset(buffer, 0, size * sizeof(char));
    if (buffer == NULL) {
        printf("Memory error.");
        return NULL;
    }
    return buffer;
}
int fileDestroy(FILE * file) {
    if (file) {
        int err;
        err = fclose(file);
        if (!err) {
            printf("The file was closed.");
            return EXIT_SUCCESS;
        }
        else {
            printf("Error in closing the file.");
            return EXIT_FAILURE;
        }
    }
}
int validateFile(char* buffer) {
    if (BFTYPE != 19778) {
        printf("This is not a BMP file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

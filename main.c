#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define IF(num) if(a >= num*16 && a<=(num+1)*16 - 1) { return num; }
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;
void copyData1(BITMAPFILEHEADER* file_Header, char* buffer) {
    memcpy(&file_Header->bfType, (const char*)buffer, 2);
    memcpy(&file_Header->bfSize, (const char*)(buffer + 2), 4);
    memcpy(&file_Header->bfReserved1, (const char*)(buffer + 6), 2);
    memcpy(&file_Header->bfReserved2, (const char*)(buffer + 8), 2);
    memcpy(&file_Header->bfOffBits, (const char*)(buffer + 10), 4);
}
void copyData2(BITMAPINFOHEADER* info_Header, char* buffer) {
    memcpy(&info_Header->biSize, (const char*)(buffer + 14), 4);
    memcpy(&info_Header->biWidth, (const char*)(buffer + 18), 4);
    memcpy(&info_Header->biHeight, (const char*)(buffer + 22), 4);
    memcpy(&info_Header->biPlanes, (const char*)(buffer + 26), 2);
    memcpy(&info_Header->biBitCount, (const char*)(buffer + 28), 2);
    memcpy(&info_Header->biCompression, (const char*)(buffer + 30), 4);
    memcpy(&info_Header->biSizeImage, (const char*)(buffer + 34), 4);
    memcpy(&info_Header->biXPelsPerMeter, (const char*)(buffer + 38), 4);
    memcpy(&info_Header->biYPelsPerMeter, (const char*)(buffer + 42), 4);
    memcpy(&info_Header->biClrUsed, (const char*)(buffer + 46), 4);
    memcpy(&info_Header->biClrImportant, (const char*)(buffer + 50), 4);
}
void printData(BITMAPFILEHEADER* file_Header, BITMAPINFOHEADER* info_Header) {
    printf("BITMAPFILEHEADER: \nbfType: \t\t0x%X (BM)\nbfSize: \t\t%d\nbfReserved1: \t\t0x%X\nbfReserved2: \t\t0x%X\nbfOffBits: \t\t%d\n\n", file_Header->bfType, file_Header->bfSize, file_Header->bfReserved1, file_Header->bfReserved2, file_Header->bfOffBits);
    printf("BITMAPINFOHEADER: \nbiSize: \t\t%d\nbiWidth: \t\t%d\nbiHeight: \t\t%d\nbiPlanes: \t\t%d\nbiBitCount: \t\t%d\nbiCompression: \t\t%d\nbiSizeImage: \t\t%d\nbiXPelsPerMeter: \t%d\nbiYPelsPerMeter: \t%d\nbiClrUsed: \t\t%d\nbiClrImportant: \t%d\n", info_Header->biSize, info_Header->biWidth, info_Header->biHeight, info_Header->biPlanes, info_Header->biBitCount, info_Header->biCompression, info_Header->biSizeImage, info_Header->biXPelsPerMeter, info_Header->biYPelsPerMeter, info_Header->biClrUsed, info_Header->biClrImportant);
}
void strreverse(char* begin, char* end) {
    char aux;
    while (end > begin)
        aux = *end, * end-- = *begin, * begin++ = aux;
}
#ifdef linux
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
    _asm mov eax, a
    _asm push ebx
    _asm push ecx
    _asm xor ebx, ebx
    _asm loop_start:
    _asm mov ecx, ebx
    _asm imul ecx, 0x00000010
    _asm cmp eax, ecx
    _asm jl loop_check
    _asm mov ecx, ebx
    _asm inc ecx
    _asm imul ecx, 0x00000010
    _asm dec ecx
    _asm cmp eax, ecx
    _asm jg loop_check
    _asm mov eax, ebx
    _asm pop ecx
    _asm pop ebx
    _asm jmp _return_
    _asm loop_check:
    _asm inc ebx
    _asm cmp ebx, 0x00000010
    _asm jb loop_start
    _asm pop ecx
    _asm pop ebx
    _asm xor eax, eax
    _asm inc eax
    _asm _return_:
    _asm mov a, eax
    return a;
}
void printIt(int* counter, int sum, const char* mess) {
    printf("\n\n%s: \n\n", mess);
    for (int i = 0, j = 0; i < 16; j++, i++)
        printf("%d-%d: %.2lf%%\n", 16 * i, 16 * i + 15, (counter[j] / (double)sum) * 100);
}
void histogramate(BITMAPFILEHEADER* file_Header, BITMAPINFOHEADER* info_Header, char* buffer) {
    int offset = file_Header->bfSize - info_Header->biSizeImage;
    int padding = ((info_Header->biWidth * 3) % 4);
    int countBlue[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countGreen[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countRed[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // for every row
    for (int i = 0; i < info_Header->biHeight; i++) {
        // for every pixel (triple)
        for (int j = 0; j < info_Header->biWidth * 3; j += 3) {
            unsigned char buffBlue;
            int b = 0;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (info_Header->biWidth * 3 + padding)), 1);
            b = checkNum(&buffBlue);
            countBlue[b]++;

            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (info_Header->biWidth * 3 + padding) + 1), 1);
            b = checkNum(&buffGreen);
            countGreen[b]++;

            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (info_Header->biWidth * 3 + padding) + 2), 1);
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
    if (info_Header->biHeight * info_Header->biWidth != sum) {
        printf("Number of pixels in the histogram does noot match number of pixels in the image");
    }
    printIt(countBlue, sum, "Blue");
    printIt(countGreen, sum, "Green");
    printIt(countRed, sum, "Red");
}
void grayScaleMe(BITMAPFILEHEADER* file_Header, BITMAPINFOHEADER* info_Header, char* buffer, FILE* rfile) {
    // copy headers
    fwrite(buffer, 14 + info_Header->biSize, 1, rfile);
    int offset = file_Header->bfSize - info_Header->biSizeImage;
    int padding = ((info_Header->biWidth * 3) % 4);
    // go through pixels
    for (int i = 0; i < info_Header->biHeight; i++) {
        for (int j = 0; j < info_Header->biWidth * 3; j += 3) {
            unsigned char buffBlue;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (info_Header->biWidth * 3 + padding)), 1);
            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (info_Header->biWidth * 3 + padding) + 1), 1);
            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (info_Header->biWidth * 3 + padding) + 2), 1);
            int mean = ((int)buffBlue + (int)buffGreen + (int)buffRed) / 3;
            // append to the file
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
int main(int argc, char** argv) {
    // Open for read in binary mode
    FILE* file = NULL;

    file = fopen(argv[1], "rb");
    if (!file) {
        printf("The file was not opened\n");
        return EXIT_FAILURE;
    }

    // obtain file size
    long fsize = 0;
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);

    // allocate memory for the file
    char* buffer;
    buffer = (char*)malloc(fsize * sizeof(char));
    memset(buffer, 0, fsize * sizeof(char));
    if (buffer == NULL) {
        printf("Memory error.");
        return EXIT_FAILURE;
    }

    // copy the content to the buffer
    size_t numberOfRead;
    numberOfRead = fread(buffer, 1, sizeof(char) * fsize, file);
    if (numberOfRead != fsize) {
        printf("Reading error.");
    }

    // copying data to the first structure
    BITMAPFILEHEADER fileHeader;
    copyData1(&fileHeader, buffer);

    // copying data to the second structure
    BITMAPINFOHEADER infoHeader;
    copyData2(&infoHeader, buffer);

    // print data from requirements if 1 command line argument
    if (argc == 2) {
        printData(&fileHeader, &infoHeader);
    }

    // generate histogram if 1 command line argument
    if (argc == 2) {
        if (infoHeader.biCompression == 0 && infoHeader.biBitCount == 24)
            histogramate(&fileHeader, &infoHeader, buffer);
        else
            printf("Histogram calculation is unsupported.");
    }
    //turn into grayscale
    if (argc == 3) {
        // Open for write in binary mode
        FILE* resultFile = NULL;
        resultFile = fopen(argv[2], "wb");
        if (!resultFile) {
            printf("The file was not opened\n");
            return EXIT_FAILURE;
        }

        grayScaleMe(&fileHeader, &infoHeader, buffer, resultFile);

        // Close the stream if it is not null
        if (resultFile) {
            int err;
            err = fclose(resultFile);
            if (!err) {
                printf("The file was closed.");
            }
            else {
                printf("Error in closing the file.");
                return EXIT_FAILURE;
            }
        }
    }
    // Close the stream if it is not null
    if (file) {
        int err;
        err = fclose(file);
        if (!err) {
            printf("The file was closed.");
        }
        else {
            printf("Error in closing the file.");
            return EXIT_FAILURE;
        }
    }
    free(buffer);
    return EXIT_SUCCESS;
}

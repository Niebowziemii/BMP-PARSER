#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define IF(num) if(a >= num*16 && a<=(num+1)*16 - 1) { return num; }
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef struct tagBITMAPFILEHEADER {
    WORD  bfType=0;
    DWORD bfSize=0;
    WORD  bfReserved1=0;
    WORD  bfReserved2=0;
    DWORD bfOffBits=0;
} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize=0;
    LONG  biWidth=0;
    LONG  biHeight=0;
    WORD  biPlanes=0;
    WORD  biBitCount=0;
    DWORD biCompression=0;
    DWORD biSizeImage=0;
    LONG  biXPelsPerMeter=0;
    LONG  biYPelsPerMeter=0;
    DWORD biClrUsed=0;
    DWORD biClrImportant=0;
} BITMAPINFOHEADER;
void copyData1(BITMAPFILEHEADER & file_Header,char* buffer) {
    memcpy(&file_Header.bfType, (const char*)buffer, 2);
    memcpy(&file_Header.bfSize, (const char*)(buffer + 2), 4);
    memcpy(&file_Header.bfReserved1, (const char*)(buffer + 6), 2);
    memcpy(&file_Header.bfReserved2, (const char*)(buffer + 8), 2);
    memcpy(&file_Header.bfOffBits, (const char*)(buffer + 10), 4);
}
void copyData2(BITMAPINFOHEADER& info_Header, char* buffer) {
    memcpy(&info_Header.biSize, (const char*)(buffer + 14), 4);
    memcpy(&info_Header.biWidth, (const char*)(buffer + 18), 4);
    memcpy(&info_Header.biHeight, (const char*)(buffer + 22), 4);
    memcpy(&info_Header.biPlanes, (const char*)(buffer + 26), 2);
    memcpy(&info_Header.biBitCount, (const char*)(buffer + 28), 2);
    memcpy(&info_Header.biCompression, (const char*)(buffer + 30), 4);
    memcpy(&info_Header.biSizeImage, (const char*)(buffer + 34), 4);
    memcpy(&info_Header.biXPelsPerMeter, (const char*)(buffer + 38), 4);
    memcpy(&info_Header.biYPelsPerMeter, (const char*)(buffer + 42), 4);
    memcpy(&info_Header.biClrUsed, (const char*)(buffer + 46), 4);
    memcpy(&info_Header.biClrImportant, (const char*)(buffer + 50), 4);
}
void printData(BITMAPFILEHEADER& file_Header, BITMAPINFOHEADER& info_Header) {
    printf("BITMAPFILEHEADER: \nbfType: \t\t0x%X (BM)\nbfSize: \t\t%d\nbfReserved1: \t\t0x%X\nbfReserved2: \t\t0x%X\nbfOffBits: \t\t%d\n\n", file_Header.bfType, file_Header.bfSize, file_Header.bfReserved1, file_Header.bfReserved2, file_Header.bfOffBits);
    printf("BITMAPINFOHEADER: \nbiSize: \t\t%d\nbiWidth: \t\t%d\nbiHeight: \t\t%d\nbiPlanes: \t\t%d\nbiBitCount: \t\t%d\nbiCompression: \t\t%d\nbiSizeImage: \t\t%d\nbiXPelsPerMeter: \t%d\nbiYPelsPerMeter: \t%d\nbiClrUsed: \t\t%d\nbiClrImportant: \t%d\n", info_Header.biSize, info_Header.biWidth, info_Header.biHeight, info_Header.biPlanes, info_Header.biBitCount, info_Header.biCompression, info_Header.biSizeImage, info_Header.biXPelsPerMeter, info_Header.biYPelsPerMeter, info_Header.biClrUsed, info_Header.biClrImportant);
}
int checkNum(unsigned char& num) {
    int a = int(num);
    for (int i = 0; i < 16; i++) {
        IF(i);
    }
    return EXIT_FAILURE;
}
void printIt(int* counter, int sum, const char* mess) {
    printf("\n\n%s: \n\n", mess);
    for (int i = 0,j=0; i < 16;j++, i ++)
        printf("%d-%d: %.2lf%%\n", 16*i,16*i+15, (counter[j] / (double)sum) * 100);
}
void histogramate(BITMAPFILEHEADER& file_Header, BITMAPINFOHEADER& info_Header, char* buffer) {
    int offset = file_Header.bfSize - info_Header.biSizeImage;
    int padding =((info_Header.biWidth*3) % 4);
    int countBlue[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countGreen[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countRed[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // for every row
    for (int i = 0; i < info_Header.biHeight; i++) {
        // for every pixel (triple)
        for (int j = 0; j < info_Header.biWidth*3; j += 3) {
            unsigned char buffBlue;
            int b=0;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (info_Header.biWidth*3 + padding)), 1);
            b = checkNum(buffBlue);
            countBlue[b]++;

            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (info_Header.biWidth*3 + padding) + 1), 1);
            b = checkNum(buffGreen);
            countGreen[b]++;

            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (info_Header.biWidth*3 + padding) + 2), 1);
            b = checkNum(buffRed);
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
    if (info_Header.biHeight * info_Header.biWidth != sum) {
        printf("Number of pixels in the histogram does noot match number of pixels in the image");
    }
    printIt(countBlue, sum, "Blue");
    printIt(countGreen, sum, "Green");
    printIt(countRed, sum, "Red");
}
void grayScaleMe(BITMAPFILEHEADER& file_Header, BITMAPINFOHEADER& info_Header, char* buffer, FILE* rfile) {
    // copy headers
    fwrite(buffer, 14 + info_Header.biSize, 1, rfile);
    int offset = file_Header.bfSize - info_Header.biSizeImage;
    int padding = ((info_Header.biWidth * 3) % 4);
    // go through pixels
    for (int i = 0; i < info_Header.biHeight; i++) {
        for (int j = 0; j < info_Header.biWidth * 3; j += 3) {
            unsigned char buffBlue;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (info_Header.biWidth * 3 + padding)), 1);
            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (info_Header.biWidth * 3 + padding) + 1), 1);
            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (info_Header.biWidth * 3 + padding) + 2), 1);
            int mean =  (int(buffBlue) + int(buffGreen) + int(buffRed)) / 3;
            // append to the file
            char g[3];
            _itoa_s(mean, g, 16);
            fwrite(g, 1, 1, rfile);
            fwrite(g, 1, 1, rfile);
            fwrite(g, 1, 1, rfile);
        }
    }
}
int main(int argc, char** argv) {
    // Open for read in binary mode
    FILE* file = NULL;
    errno_t err = NULL;
    err = fopen_s(&file, argv[1], "rb");
    if (err) {
        printf("The file was not opened\n");
        return EXIT_FAILURE;
    }

    // obtain file size
    long fsize = NULL;
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
    numberOfRead = fread_s(buffer, sizeof(char) * fsize, 1, sizeof(char) * fsize, file);
    if (numberOfRead != fsize) {
        printf("Reading error.");
    }

    // copying data to the first structure
    BITMAPFILEHEADER fileHeader;
    copyData1(fileHeader, buffer);

    // copying data to the second structure
    BITMAPINFOHEADER infoHeader;
    copyData2(infoHeader, buffer);

    // print data from requirements if 1 command line argument
    if (argc == 2) {
        printData(fileHeader, infoHeader);
    }

    // generate histogram if 1 command line argument
    if (argc == 2) {
        if (infoHeader.biCompression != '0' || infoHeader.biBitCount != '24')
            histogramate(fileHeader, infoHeader, buffer);
        else
            printf("Histogram claculation is unsupported.");
    }
    //turn into grayscale
    if (argc == 3) {
        // Open for write in binary mode
        FILE* rfile = NULL;
        errno_t err = NULL;
        err = fopen_s(&rfile, argv[2], "wb");
        if (err) {
            printf("The file was not opened\n");
            return EXIT_FAILURE;
        }
        
        grayScaleMe(fileHeader, infoHeader, buffer,rfile);

        // Close the stream if it is not null
        if (rfile) {
            err = fclose(rfile);
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

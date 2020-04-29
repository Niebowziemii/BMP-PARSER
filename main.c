#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
void copyData1(BITMAPFILEHEADER & xd,char* buffer) {
    memcpy(&xd.bfType, (const char*)buffer, 2);
    memcpy(&xd.bfSize, (const char*)(buffer + 2), 4);
    memcpy(&xd.bfReserved1, (const char*)(buffer + 6), 2);
    memcpy(&xd.bfReserved2, (const char*)(buffer + 8), 2);
    memcpy(&xd.bfOffBits, (const char*)(buffer + 10), 4);
}
void copyData2(BITMAPINFOHEADER& xdd, char* buffer) {
    memcpy(&xdd.biSize, (const char*)(buffer + 14), 4);
    memcpy(&xdd.biWidth, (const char*)(buffer + 18), 4);
    memcpy(&xdd.biHeight, (const char*)(buffer + 22), 4);
    memcpy(&xdd.biPlanes, (const char*)(buffer + 26), 2);
    memcpy(&xdd.biBitCount, (const char*)(buffer + 28), 2);
    memcpy(&xdd.biCompression, (const char*)(buffer + 30), 4);
    memcpy(&xdd.biSizeImage, (const char*)(buffer + 34), 4);
    memcpy(&xdd.biXPelsPerMeter, (const char*)(buffer + 38), 4);
    memcpy(&xdd.biYPelsPerMeter, (const char*)(buffer + 42), 4);
    memcpy(&xdd.biClrUsed, (const char*)(buffer + 46), 4);
    memcpy(&xdd.biClrImportant, (const char*)(buffer + 50), 4);
}
void printData(BITMAPFILEHEADER& xd, BITMAPINFOHEADER& xdd) {
    printf("BITMAPFILEHEADER: \nbfType: \t\t0x%X (BM)\nbfSize: \t\t%d\nbfReserved1: \t\t0x%X\nbfReserved2: \t\t0x%X\nbfOffBits: \t\t%d\n\n", xd.bfType, xd.bfSize, xd.bfReserved1, xd.bfReserved2, xd.bfOffBits);
    printf("BITMAPINFOHEADER: \nbiSize: \t\t%d\nbiWidth: \t\t%d\nbiHeight: \t\t%d\nbiPlanes: \t\t%d\nbiBitCount: \t\t%d\nbiCompression: \t\t%d\nbiSizeImage: \t\t%d\nbiXPelsPerMeter: \t%d\nbiYPelsPerMeter: \t%d\nbiClrUsed: \t\t%d\nbiClrImportant: \t%d\n", xdd.biSize, xdd.biWidth, xdd.biHeight, xdd.biPlanes, xdd.biBitCount, xdd.biCompression, xdd.biSizeImage, xdd.biXPelsPerMeter, xdd.biYPelsPerMeter, xdd.biClrUsed, xdd.biClrImportant);
}
int checkNum(unsigned char& num) {
    int a = int(num);
    if (a >= 0 && a <= 15) {
        return 0;
    }
    else if (a >= 16 && a <= 31) {
        return 1;
    }
    else if (a >= 32 && a <= 47) {
        return 2;
    }
    else if (a >= 48 && a <= 63) {
        return 3;
    }
    else if (a >= 64 && a <= 79) {
        return 4;
    }
    else if (a >= 80 && a <= 95) {
        return 5;
    }
    else if (a >= 96 && a <= 111) {
        return 6;
    }
    else if (a >= 112 && a <= 127) {
        return 7;
    }
    else if (a >= 128 && a <= 143) {
        return 8;
    }
    else if (a >= 144 && a <= 159) {
        return 9;
    }
    else if (a >= 160 && a <= 175) {
        return 10;
    }
    else if (a >= 176 && a <= 191) {
        return 11;
    }
    else if (a >= 192 && a <= 207) {
        return 12;
    }
    else if (a >= 208 && a <= 223) {
        return 13;
    }
    else if (a >= 224 && a <= 239) {
        return 14;
    }
    else if (a >= 240 && a <= 255) {
        return 15;
    }
    else {
        printf("Something's bad.");
        return EXIT_FAILURE;
    }
}
void printIt(int* counter, int sum, const char* mess) {
    printf("\n\n%s: \n\n", mess);
    printf("0-15: %.2lf%%\n", (counter[0] / (double)sum)*100);
    printf("16-31: %.2lf%%\n", (counter[1] / (double)sum) * 100);
    printf("32-47: %.2lf%%\n", (counter[2] / (double)sum) * 100);
    printf("48-63: %.2lf%%\n", (counter[3] / (double)sum) * 100);
    printf("64-79: %.2lf%%\n", (counter[4] / (double)sum) * 100);
    printf("80-95: %.2lf%%\n", (counter[5] / (double)sum) * 100);
    printf("96-111: %.2lf%%\n", (counter[6] / (double)sum) * 100);
    printf("112-127: %.2lf%%\n", (counter[7] / (double)sum) * 100);
    printf("128-143: %.2lf%%\n", (counter[8] / (double)sum) * 100);
    printf("144-156: %.2lf%%\n", (counter[9] / (double)sum) * 100);
    printf("160-175: %.2lf%%\n", (counter[10] / (double)sum) * 100);
    printf("176-191: %.2lf%%\n", (counter[11] / (double)sum) * 100);
    printf("192-207: %.2lf%%\n", (counter[12] / (double)sum) * 100);
    printf("208-223: %.2lf%%\n", (counter[13] / (double)sum) * 100);
    printf("224-239: %.2lf%%\n", (counter[14] / (double)sum) * 100);
    printf("240-255: %.2lf%%\n", (counter[15] / (double)sum) * 100);

}
void histogramate(BITMAPFILEHEADER& xd, BITMAPINFOHEADER& xdd, char* buffer) {
    int offset = xd.bfSize - xdd.biSizeImage;
    int padding =((xdd.biWidth*3) % 4);
    int countBlue[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countGreen[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int countRed[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // for every row
    for (int i = 0; i < xdd.biHeight; i++) {
        // for every pixel (triple)
        for (int j = 0; j < xdd.biWidth*3; j += 3) {
            unsigned char buffBlue;
            int b=0;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (xdd.biWidth*3 + padding)), 1);
            b = checkNum(buffBlue);
            countBlue[b]++;

            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (xdd.biWidth*3 + padding) + 1), 1);
            b = checkNum(buffGreen);
            countGreen[b]++;

            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (xdd.biWidth*3 + padding) + 2), 1);
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
    if (xdd.biHeight * xdd.biWidth != sum) {
        printf("Number of pixels in the histogram does noot match number of pixels in the image");
    }
    printIt(countBlue, sum, "Blue");
    printIt(countGreen, sum, "Green");
    printIt(countRed, sum, "Red");
}
void grayScaleMe(BITMAPFILEHEADER& xd, BITMAPINFOHEADER& xdd, char* buffer, FILE* rfile) {
    // copy headers
    fwrite(buffer, 14 + xdd.biSize, 1, rfile);
    int offset = xd.bfSize - xdd.biSizeImage;
    int padding = ((xdd.biWidth * 3) % 4);
    // go through pixels
    for (int i = 0; i < xdd.biHeight; i++) {
        for (int j = 0; j < xdd.biWidth * 3; j += 3) {
            unsigned char buffBlue;
            memset(&buffBlue, 0, sizeof(char));
            memcpy(&buffBlue, (const char*)(buffer + offset + j + i * (xdd.biWidth * 3 + padding)), 1);
            unsigned char buffGreen;
            memset(&buffGreen, 0, sizeof(char));
            memcpy(&buffGreen, (const char*)(buffer + offset + j + i * (xdd.biWidth * 3 + padding) + 1), 1);
            unsigned char buffRed;
            memset(&buffRed, 0, sizeof(char));
            memcpy(&buffRed, (const char*)(buffer + offset + j + i * (xdd.biWidth * 3 + padding) + 2), 1);
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
//DRIVER CODE
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
    BITMAPFILEHEADER XD;
    copyData1(XD, buffer);

    // copying data to the second structure
    BITMAPINFOHEADER XDD;
    copyData2(XDD, buffer);

    // print data from requirements if 1 command line argument
    if (argc == 2) {
        printData(XD, XDD);
    }

    // generate histogram if 1 command line argument
    if (argc == 2) {
        if (XDD.biCompression != 0 || XDD.biBitCount != 24)
            histogramate(XD, XDD, buffer);
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
        
        grayScaleMe(XD, XDD, buffer,rfile);

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

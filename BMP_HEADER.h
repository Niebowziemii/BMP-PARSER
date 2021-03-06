/*MIT License

Copyright (c) 2020: 
Rafał Niebowziemii Stachowiak

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#pragma once
#if defined(_WIN32)||defined(WIN32)\
|| defined(_WIN64)||defined(WIN64)
#error Windows is not supported by this program, please use LINUX instead
#endif
#if defined(__APPLE__)||defined(__MACH__)
#error Mac OS is not supported by this program, please use LINUX instead
#endif
#if defined(unix)||defined(__unix__)\
||defined(__unix)||defined(__linux__)\
||defined(linux)||defined(__linux)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


#ifdef BFTYPE
#undef BFTYPE
#endif // !BFTYPE
#define BFTYPE(name) *((uint16_t*)&name[0])

#ifdef BFSIZE
#undef BFSIZE
#endif // !BFSIZE
#define BFSIZE(name) *((uint32_t*)&name[2])

#ifdef BFRESERVED1
#undef BFRESERVED1
#endif // !BFRESERVED1
#define BFRESERVED1(name) *((uint16_t*)&name[6])

#ifdef BFRESERVED2
#undef BFRESERVED2
#endif // !BFRESERVED2
#define BFRESERVED2(name) *((uint16_t*)&name[8])

#ifdef BFOFFBITS
#undef BFOFFBITS
#endif // !BFOFFBITS
#define BFOFFBITS(name) *((uint32_t*)&name[10])

#ifdef BISIZE
#undef BISIZE
#endif // !BISIZE
#define BISIZE(name) *((uint32_t*)&name[14])

#ifdef BIWIDTH
#undef BIWIDTH
#endif // !BIWIDTH
#define BIWIDTH(name) *((uint32_t*)&name[18])

#ifdef BIHEIGHT
#undef BIHEIGHT
#endif // !BIHEIGHT
#define BIHEIGHT(name) *((uint32_t*)&name[22])

#ifdef BIPLANES
#undef BIPLANES
#endif// !BIPLANES
#define BIPLANES(name) *((uint16_t*)&name[26])

#ifdef BIBITCOUNT
#undef BIBITCOUNT
#endif// !BIBITCOUNT
#define BIBITCOUNT(name) *((uint16_t*)&name[28])

#ifdef BICOMPRESSION
#undef BICOMPRESSION
#endif// !BICOMPRESSION
#define BICOMPRESSION(name) *((uint32_t*)&name[30])

#ifdef BISIZEIMAGE
#undef BISIZEIMAGE
#endif// !BISIZEIMAGE
#define BISIZEIMAGE(name) *((uint32_t*)&name[34])

#ifdef BIXPERLSPERMETER
#undef BIXPERLSPERMETER
#endif// !BIXPERLSPERMETER
#define BIXPERLSPERMETER(name) *((uint32_t*)&name[38])

#ifdef BIYPERLSPERMETER
#undef BIYPERLSPERMETER
#endif// !BIYPERLSPERMETER
#define BIYPERLSPERMETER(name) *((uint32_t*)&name[42])

#ifdef BICLRUSED
#undef BICLRUSED
#endif// !BICLRUSED
#define BICLRUSED(name) *((uint32_t*)&name[46])

#ifdef BICLRIMPORTANT
#undef BICLRIMPORTANT
#endif// !BICLRIMPORTANT
#define BICLRIMPORTANT(name) *((uint32_t*)&name[50])

void print_data(char* buffer) {
    printf("BITMAPFILEHEADER:\
\
\nbf_type: \t\t0x%X\t%d (BM)\n\
bf_size: \t\t0x%X\t%d\n\
bf_reserved1: \t\t0x%X\t%d\n\
bf_reserved2: \t\t0x%X\t%d\n\
bf_off_bits: \t\t0x%X\t%d\n\n", \
BFTYPE(buffer), BFTYPE(buffer), \
BFSIZE(buffer), BFSIZE(buffer), \
BFRESERVED1(buffer), BFRESERVED1(buffer), \
BFRESERVED2(buffer), BFRESERVED2(buffer), \
BFOFFBITS(buffer), BFOFFBITS(buffer));

    printf("BITMAPINFOHEADER:\
\
\nbi_size: \t\t0x%X\t%d\n\
bi_width: \t\t0x%X\t%d\n\
bi_height: \t\t0x%X\t%d\n\
bi_planes: \t\t0x%X\t%d\n\
bi_bit_count: \t\t0x%X\t%d\n\
bi_compression: \t0x%X\t%d\n\
bi_size_image: \t\t0x%X\t%d\n\
bi_x_pels_per_meter: \t0x%X\t%d\n\
bi_y_pels_per_meter: \t0x%X\t%d\n\
bi_clr_used: \t\t0x%X\t%d\n\
bi_clr_important: \t0x%X\t%d\n", \
BISIZE(buffer), BISIZE(buffer), \
BIWIDTH(buffer), BIWIDTH(buffer), \
BIHEIGHT(buffer), BIHEIGHT(buffer), \
BIPLANES(buffer), BIPLANES(buffer), \
BIBITCOUNT(buffer), BIBITCOUNT(buffer), \
BICOMPRESSION(buffer), BICOMPRESSION(buffer), \
BISIZEIMAGE(buffer), BISIZEIMAGE(buffer), \
BIXPERLSPERMETER(buffer), BIXPERLSPERMETER(buffer), \
BIYPERLSPERMETER(buffer), BIYPERLSPERMETER(buffer), \
BICLRUSED(buffer), BICLRUSED(buffer), \
BICLRIMPORTANT(buffer), BICLRIMPORTANT(buffer));
}
void print_histogram_colour_data(uint32_t* counter, int sum, const char* mess) {
    printf("\n\n%s: \n\n", mess);
    for (int i = 0, j = 0; i < 16; j++, i++)
        printf("%d-%d: %.2lf%%\n", 16 * i, 16 * i + 15, (counter[j] / (double)sum) * 100);
}
void generate_histogram(char* buffer) {
    uint32_t offset = BFSIZE(buffer) - BISIZEIMAGE(buffer);
    uint32_t padding = ((BIWIDTH(buffer) * 3) % 4);
    uint32_t count[48];
    uint32_t buffer_offset = 0;
    memset(&count, 0, sizeof(int) * 48);
    // for every row
    for (unsigned int i = 0; i < BIHEIGHT(buffer); i++) {
        // for every pixel (triple)
        for (unsigned int j = 0; j < BIWIDTH(buffer) * 3; j += 3) {
            buffer_offset = offset + j + i * (BIWIDTH(buffer) * 3 + padding);
            unsigned char buff_blue = (unsigned char)(buffer[buffer_offset]);
            count[buff_blue / 16]++;

            unsigned char buff_green = (unsigned char)(buffer[buffer_offset + 1]);
            count[buff_green / 16 + 16]++;

            unsigned char buff_red = (unsigned char)(buffer[buffer_offset + 2]);
            count[buff_red / 16 + 32]++;
            //printf("Blue: %X Green: %X Red: %X\n", buff_blue, buff_green, buff_red);
        }
    }
    // obtain sum of all pixel values to check correctness 
    int sum = 0;
    for (int i = 0; i < 16; i++)
        sum += count[i];
    // handle error
    if (BIHEIGHT(buffer) * BIWIDTH(buffer) != sum) {
        printf("[error]Number of pixels in the histogram does not match number of pixels in the image");
    }
    print_histogram_colour_data(count, sum, "Blue");
    print_histogram_colour_data((count + 16), sum, "Green");
    print_histogram_colour_data((count + 32), sum, "Red");
}
void create_a_copy_of_given_file_in_grayscale(char* buffer, FILE* rfile) {
    // copy headers
    fwrite(buffer, 14 + BISIZE(buffer), 1, rfile);
    uint32_t offset = BFSIZE(buffer) - BISIZEIMAGE(buffer);
    uint32_t padding = ((BIWIDTH(buffer) * 3) % 4);
    uint32_t buffer_offset = 0;
    // insert pixels
    for (unsigned i = 0; i < BIHEIGHT(buffer); i++) {
        for (unsigned j = 0; j < BIWIDTH(buffer) * 3; j += 3) {
            buffer_offset = offset + j + i * (BIWIDTH(buffer) * 3 + padding);
            unsigned char buff_blue = (unsigned char)(buffer[buffer_offset]);
            unsigned char buff_green = (unsigned char)(buffer[buffer_offset + 1]);
            unsigned char buff_red = (unsigned char)(buffer[buffer_offset + 2]);
            const char mean = (buff_blue + buff_green + buff_red) / 3;
            fwrite(&mean, 1, 1, rfile);
            fwrite(&mean, 1, 1, rfile);
            fwrite(&mean, 1, 1, rfile);
        }
    }
}
FILE* create_a_file_from_name_stored_in_main_argument_list(char** argv, int index, const char* mode) {
    FILE* file = NULL;
    if (argv[index]) {
        file = fopen(argv[index], mode);
        if (!file) {
            printf("[error]The file was not opened\n");
            return NULL;
        }
        return file;
    }
    else {
        fprintf(stderr, "Too few command line arguments");
        return NULL;
    }
}

int validate_file(char* buffer) {
    if (BFTYPE(buffer) != 19778) {
        printf("[error]This is not a BMP file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
#endif

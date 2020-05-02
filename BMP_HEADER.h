#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

#ifndef IF(num)
#define IF(num) if(a >= num*16 && a<=(num+1)*16 - 1) { return num; }
#endif
void print_data(char* buffer) {
    printf("BITMAPFILEHEADER:\
\
\nbf_type: \t\t0x%X\t%d (BM)\n\
bf_size: \t\t0x%X\t%d\n\
bf_reserved1: \t\t0x%X\t%d\n\
bf_reserved2: \t\t0x%X\t%d\n\
bf_off_bits: \t\t0x%X\t%d\n\n",\
BFTYPE(buffer), BFTYPE(buffer),\
BFSIZE(buffer), BFSIZE(buffer),\
BFRESERVED1(buffer), BFRESERVED1(buffer),\
BFRESERVED2(buffer), BFRESERVED2(buffer),\
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
bi_clr_important: \t0x%X\t%d\n",\
BISIZE(buffer), BISIZE(buffer),\
BIWIDTH(buffer), BIWIDTH(buffer),\
BIHEIGHT(buffer), BIHEIGHT(buffer),\
BIPLANES(buffer), BIPLANES(buffer),\
BIBITCOUNT(buffer), BIBITCOUNT(buffer),\
BICOMPRESSION(buffer), BICOMPRESSION(buffer),\
BISIZEIMAGE(buffer), BISIZEIMAGE(buffer),\
BIXPERLSPERMETER(buffer), BIXPERLSPERMETER(buffer),\
BIYPERLSPERMETER(buffer), BIYPERLSPERMETER(buffer),\
BICLRUSED(buffer), BICLRUSED(buffer),\
BICLRIMPORTANT(buffer), BICLRIMPORTANT(buffer));
}
int check_num(unsigned char* num) {
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
void print_histogram_colour_data(int* counter, int sum, const char* mess) {
    printf("\n\n%s: \n\n", mess);
    for (int i = 0, j = 0; i < 16; j++, i++)
        printf("%d-%d: %.2lf%%\n", 16 * i, 16 * i + 15, (counter[j] / (double)sum) * 100);
}
void generate_histogram(char* buffer) {
    int offset = BFSIZE(buffer) - BISIZEIMAGE(buffer);
    int padding = ((BIWIDTH(buffer) * 3) % 4);
    int count_blue[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int count_green[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int count_red[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // for every row
    for (unsigned i = 0; i < BIHEIGHT(buffer); i++) {
        // for every pixel (triple)
        for (unsigned j = 0; j < BIWIDTH(buffer) * 3; j += 3) {
            unsigned char buff_blue = (unsigned char)(buffer[offset + j + i * (BIWIDTH(buffer) * 3 + padding)]);
            int b = 0;
            b = check_num(&buff_blue);
            count_blue[b]++;

            unsigned char buff_green = (unsigned char)(buffer[offset + j + i * (BIWIDTH(buffer) * 3 + padding)+1]);
            b = check_num(&buff_green);
            count_green[b]++;

            unsigned char buff_red = (unsigned char)(buffer[offset + j + i * (BIWIDTH(buffer) * 3 + padding)+2]);
            b = check_num(&buff_red);
            count_red[b]++;
            //printf("Blue: %X Green: %X Red: %X\n", buff_blue, buff_green, buff_red);
        }
    }
    // obtain sum of all pixel values to check correctness 
    int sum = 0;
    for (int i = 0; i < 16; i++)
        sum += count_blue[i];
    // handle error
    if (BIHEIGHT(buffer) * BIWIDTH(buffer) != sum) {
        printf("Number of pixels in the histogram does not match number of pixels in the image");
    }
    print_histogram_colour_data(count_blue, sum, "Blue");
    print_histogram_colour_data(count_green, sum, "Green");
    print_histogram_colour_data(count_red, sum, "Red");
}
void turn_file_into_grayscale(char* buffer, FILE * rfile) {
    // copy headers
    fwrite(buffer, 14 + BISIZE(buffer), 1, rfile);
    int offset = BFSIZE(buffer) - BISIZEIMAGE(buffer);
    int padding = ((BIWIDTH(buffer) * 3) % 4);
    // insert pixels
    for (int i = 0; i < BIHEIGHT(buffer); i++) {
        for (int j = 0; j < BIWIDTH(buffer) * 3; j += 3) {
            unsigned char buff_blue = (unsigned char)(buffer[offset + j + i * (BIWIDTH(buffer) * 3 + padding)]);
            unsigned char buff_green = (unsigned char)(buffer[offset + j + i * (BIWIDTH(buffer) * 3 + padding) + 1]);
            unsigned char buff_red = (unsigned char)(buffer[offset + j + i * (BIWIDTH(buffer) * 3 + padding) + 2]);
            const char mean = (buff_blue + buff_green + buff_red) / 3;
            fwrite(&mean, 1, 1, rfile);
            fwrite(&mean, 1, 1, rfile);
            fwrite(&mean, 1, 1, rfile);
        }
    }
}
FILE* create_a_file_from_name_stored_in_main_argument_list(char** argv, int index, const char* mode) {
    FILE* file = NULL;
    file = fopen(argv[index], mode);
    if (!file) {
        printf("The file was not opened\n");
        return NULL;
    }
    return file;
}

int validate_file(char* buffer) {
    if (BFTYPE(buffer) != 19778) {
        printf("This is not a BMP file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

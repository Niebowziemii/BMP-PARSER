/*
MIT License
Copyright(c) 2020 Franciszek Olejnik (FrankOil)
https://github.com/FrankOil/INIParser

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions
The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#if !defined(BMP_PARSER_LIBRARY)
#define BMP_PARSER_LIBRARY

#define _CRT_SECURE_NO_WARNINGS

//include critical libraries
#include <stdio.h>
#include <stdint.h>

typedef struct BMPFile
{
    unsigned char* data;
    uint32_t file_size;
} BMPFile;

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
typedef struct BMPFileHeader
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;

// https://docs.microsoft.com/pl-pl/previous-versions/dd183376(v=vs.85)
typedef struct BMPFileInfo
{
    // BITMAPCOREHEADER
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;

    // BITMAPINFOHEADER
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;

    // TODO BITMAPV4HEADER
    // TODO BITMAPV5HEADER
} BMPFileInfo;

// disable fopen warning (if fopen_s is available then we have to use it)
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE

// ensure the existence of EXIT_SUCCESS
#if !defined(EXIT_SUCCESS)
#define EXIT_SUCCESS 0
#endif

// ensure the existence of EXIT_FAILURE
#if !defined(EXIT_FAILURE)
#define EXIT_FAILURE 1
#endif

// ensure the existence of NULL
#if !defined(NULL)
#define NULL 0
#endif

/*======== check if the build target is set to the Windows =======*/
#if defined(_WIN64) || defined(_WIN32)

// define BMP_PARSER_API_WINDOWS
// required to generate winapi code
#if defined(BMP_PARSER_API_WINDOWS)
#undef BMP_PARSER_API_WINDOWS
#endif
#define BMP_PARSER_API_WINDOWS 1

// include windows api
#include <windows.h>

// force the CreateFile version compatible with char* 
#if defined(CreateFile)
#undef CreateFile
#define CreateFile  CreateFileA
#endif

/*===== check if the build target is set to a UNIX-like system =====*/
#elif defined(unix) || defined(__unix__) \
|| defined(__unix) || defined(__linux__) \
|| defined(__linux) || defined (linux) \
|| defined(__GNU__) || defined (__gnu_linux__)

// define BMP_PARSER_API_POSIX
// required for fstat
#if defined(BMP_PARSER_API_POSIX)
#undef BMP_PARSER_API_POSIX
#endif
#define BMP_PARSER_API_POSIX 1

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

/*====================== any other build target =====================*/
#else

// define BMP_PARSER_API_NONE
// if the target is not set to the Windows or a Unix-like system
// then we use the fseek function to calculcate a file size
#if defined(BMP_PARSER_API_NONE)
#undef BMP_PARSER_API_NONE
#endif
#define BMP_PARSER_API_NONE 1

#include <string.h>
#include <stdlib.h>

#endif

// define a shortcut for error handling
#if defined(BMP_PARSER_RETURN_ERROR)
#undef BMP_PARSER_RETURN_ERROR
#endif
#define BMP_PARSER_RETURN_ERROR(message) \
fprintf(stderr, message); \
printf("\n"); \
return EXIT_FAILURE;

// BMP_PARSER_OBTAIN_FILE_DATA cannot be predefined
#if defined(BMP_PARSER_OBTAIN_FILE_DATA)
#undef BMP_PARSER_OBTAIN_FILE_DATA
#endif

/*======== generate WindowsAPI-specific code  =======*/
#if defined(BMP_PARSER_API_WINDOWS)

#define BMP_PARSER_OBTAIN_FILE_DATA(file_name) \
HANDLE file = CreateFile(file_name, GENERIC_READ, NULL, \
NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); \
if (file == INVALID_HANDLE_VALUE) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to open a file!"); \
} \
LARGE_INTEGER large_file_size; \
if (!GetFileSizeEx(file, &large_file_size)) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to obtain the file size!"); \
} \
if (large_file_size.HighPart) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to read file larger than 4GB!"); \
} \
uint32_t file_size = large_file_size.LowPart; \
uint8_t* file_content = (uint8_t *)malloc(file_size * sizeof(uint8_t)); \
if (file_content == NULL) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to allocate memory!"); \
} \
DWORD total_bytes_read = NULL; \
if(!ReadFile(file, file_content, file_size, &total_bytes_read, NULL)) \
{\
    BMP_PARSER_RETURN_ERROR("Error: Unable to read the file!"); \
}\
if (total_bytes_read != file_size) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to read the whole file!"); \
} \
CloseHandle(file);

/*======== generate nonWindowsAPI-specific code  =======*/
#else

// BMP_PARSER_OBTAIN_FILE_SIZE cannot be predefined
#if !defined(BMP_PARSER_OBTAIN_FILE_SIZE)
#undef BMP_PARSER_OBTAIN_FILE_SIZE
#endif

#if defined(BMP_PARSER_API_POSIX)

#define BMP_PARSER_OBTAIN_FILE_SIZE() \
struct stat file_info; \
if(fstat(fileno(file), &file_info)) \
{ \
    BMP_PARSER_RETURN_ERROR("Errro: Unable to obtain the file size"); \
} \
if(!S_ISREG(file_info.st_mode)) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: The given path does not point to the file"); \
} \
if(!file_info.st_size) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Given file has no size. Unable to copy"); \
} \
if (file_info.st_size > UINT32_MAX) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to read file larger than 4GB!"); \
} \
uint32_t file_size = file_info.st_size; \

#else

#define BMP_PARSER_OBTAIN_FILE_SIZE() \
fseek(file, 0, SEEK_END); \
uint64_t file_size_long = ftell(file); \
fseek(file, 0, SEEK_SET); \
if(!file_size_long) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Given file has no size. Unable to copy"); \
} \
if (file_size_long > UINT32_MAX) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to read file larger than 4GB!"); \
} \
uint32_t file_size = file_size_long; \

#endif

#define BMP_PARSER_OBTAIN_FILE_DATA(file_name) \
FILE * file = fopen(file_name, "rb"); \
if (file == NULL) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to open a file!"); \
} \
BMP_PARSER_OBTAIN_FILE_SIZE() \
uint8_t* file_content = (uint8_t *)malloc(file_size * sizeof(uint8_t)); \
if (file_content == NULL) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to allocate memory!"); \
} \
uint32_t total_bytes_read = fread(file_content, 1, file_size, file); \
if (total_bytes_read != file_size) \
{ \
    BMP_PARSER_RETURN_ERROR("Error: Unable to read the whole file!"); \
} \
fclose(file);

#endif

// casts data to uint16_t pointer
#if defined(BMP_PARSER_CAST_TO_UINT16_PTR)
#undef BMP_PARSER_CAST_TO_UINT16_PTR
#endif
#define BMP_PARSER_CAST_TO_UINT16_PTR(data) (uint16_t*)&data

// casts data to uint32_t pointer
#if defined(BMP_PARSER_CAST_TO_UINT32_PTR)
#undef BMP_PARSER_CAST_TO_UINT32_PTR
#endif
#define BMP_PARSER_CAST_TO_UINT32_PTR(data) (uint32_t*)&data

// casts data to int32_t pointer
#if defined(BMP_PARSER_CAST_TO_INT32_PTR)
#undef BMP_PARSER_CAST_TO_INT32_PTR
#endif
#define BMP_PARSER_CAST_TO_INT32_PTR(data) (int32_t*)&data

// print uint16_t data
#if defined(BMP_PARSER_PRINT_UINT16)
#undef BMP_PARSER_PRINT_UINT16
#endif
#define BMP_PARSER_PRINT_UINT16(name, offset) \
dataU16bit = BMP_PARSER_CAST_TO_UINT16_PTR(bmp_file->data[offset]); \
printf("%s: 0x%X (%u)\n", name, *dataU16bit, *dataU16bit);

// print uint32_t data
#if defined(BMP_PARSER_PRINT_UINT32)
#undef BMP_PARSER_PRINT_UINT32
#endif
#define BMP_PARSER_PRINT_UINT32(name, offset) \
dataU32bit = BMP_PARSER_CAST_TO_UINT32_PTR(bmp_file->data[offset]); \
printf("%s: 0x%X (%u)\n", name, *dataU32bit, *dataU32bit);

// print int32_t data
#if defined(BMP_PARSER_PRINT_INT32)
#undef BMP_PARSER_PRINT_INT32
#endif
#define BMP_PARSER_PRINT_INT32(name, offset) \
data32bit = BMP_PARSER_CAST_TO_INT32_PTR(bmp_file->data[offset]); \
printf("%s: 0x%X (%u)\n", name, *data32bit, *data32bit);

// generate uint16_t getter
#if defined(BMP_PARSER_GENERATE_UINT16_GETTER)
#undef BMP_PARSER_GENERATE_UINT16_GETTER
#endif
#define BMP_PARSER_GENERATE_UINT16_GETTER(attribute_name, offset) \
uint16_t bmp_get_##attribute_name(BMPFile* bmp_file) { return *BMP_PARSER_CAST_TO_UINT16_PTR(bmp_file->data[offset]); };

// generate uint32_t getter
#if defined(BMP_PARSER_GENERATE_UINT32_GETTER)
#undef BMP_PARSER_GENERATE_UINT32_GETTER
#endif
#define BMP_PARSER_GENERATE_UINT32_GETTER(attribute_name, offset) \
uint32_t bmp_get_##attribute_name(BMPFile* bmp_file) { return *BMP_PARSER_CAST_TO_UINT32_PTR(bmp_file->data[offset]); };

// generate int32_t getter
#if defined(BMP_PARSER_GENERATE_INT32_GETTER)
#undef BMP_PARSER_GENERATE_INT32_GETTER
#endif
#define BMP_PARSER_GENERATE_INT32_GETTER(attribute_name, offset) \
int32_t bmp_get_##attribute_name(BMPFile* bmp_file) { return *BMP_PARSER_CAST_TO_INT32_PTR(bmp_file->data[offset]); };

// returns EXIT_FAILURE if the value of the given byte is incorrect
#if defined(BMP_PARSER_VALIDATE_BYTE)
#undef BMP_PARSER_VALIDATE_BYTE
#endif
#define BMP_PARSER_VALIDATE_BYTE(VARIABLE, EXPECTED, MESSAGE) \
if (VARIABLE != EXPECTED) { BMP_PARSER_RETURN_ERROR(MESSAGE); }

// we check the structure of the file header based on the microsoft's documentation
// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
static inline int _validate_file_header(unsigned char* file_content, uint32_t size)
{
    // check file type, it must be BM
    BMP_PARSER_VALIDATE_BYTE(file_content[0], 'B', "First byte must be set to the 'B'!");
    BMP_PARSER_VALIDATE_BYTE(file_content[1], 'M', "First byte must be set to the 'M'!");

    // check file size
    if (memcmp(&file_content[2], &size, 4))
    {
        BMP_PARSER_RETURN_ERROR("Error: File size is different than a fize size from header!");
    }

    // check if reserved 1 and 2 are set to NULL (zero)
    uint32_t null_bytes = 0;

    if (memcmp(&file_content[6], &null_bytes, 4))
    {
        BMP_PARSER_RETURN_ERROR("Error: Reserved bytes from the file header must be set to NULL!");
    }

    // if the pixel_array offset is greater than a file size
    if (memcmp(&size, &file_content[10], 4) < 0)
    {
        BMP_PARSER_RETURN_ERROR("Error: Offset of the pixel array is greater than the file's size!");
    }
 
    return EXIT_SUCCESS;
}

// validate a header size value based on wikipedia
// https://en.wikipedia.org/wiki/BMP_file_format
static inline int _validate_info_header_size(uint32_t size)
{
    switch (size)
    {
    // supported header sizes
    // case 12: BITMAPCOREHEADER size 12 bytes
    case 40: // BITMAPINFOHEADER size 40 bytes
    case 108: // BITMAPV4HEADER size 108 bytes
    case 124: // BITMAPV5HEADER size 124 bytes
        return EXIT_SUCCESS;
    default:
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}

// we check the structure of the info header based on the microsoft's documentation
// https://docs.microsoft.com/pl-pl/previous-versions/dd183376(v=vs.85)
static inline int _validate_info_header(unsigned char* file_content, uint32_t file_size)
{
    // check if header size is correct
    uint32_t info_header_size = *BMP_PARSER_CAST_TO_UINT32_PTR(file_content[14]);
    if (_validate_info_header_size(info_header_size))
    {
        BMP_PARSER_RETURN_ERROR("Error: Unsupported header size!");
    }

    // check if a file_header_size + info_header_size > file_size
    if (info_header_size + 14 > file_size)
    {
        BMP_PARSER_RETURN_ERROR("Error: Declarated File header's size + Info header's size is greater than the file's size!");
    }

    // based on microsoft's documentation
    // "bcPlanes - The number of planes for the target device. This value must be 1."
    uint16_t bcPlanes_value = 1;
    if (memcmp(&file_content[26], &bcPlanes_value, 2))
    {
        BMP_PARSER_RETURN_ERROR("Error: bcPlanes attribute must be set to 1!");
    }

    // currently 24bit per pixel is supported
    // uint16_t biBitCount_value = 24;
    // if (memcmp(&file_content[28], &biBitCount_value, 2))
    // {
    //     BMP_PARSER_RETURN_ERROR("Error: Unsupported number of bits per pixel!");
    // }

    // currently only no-compression mode is supported
    // uint32_t biCompression_value = 0;
    // if (memcmp(&file_content[30], &biCompression_value, 4))
    // {
    //     BMP_PARSER_RETURN_ERROR("Error: Unsupported compression mode!");
    // }

    uint32_t biClrUser = *BMP_PARSER_CAST_TO_UINT32_PTR(file_content[46]);
    uint32_t bfOffBits = *BMP_PARSER_CAST_TO_UINT32_PTR(file_content[10]);
    // check if file_header_size + info_header_size + biClrUser == pixel offset
    if (info_header_size + 14 + biClrUser != bfOffBits)
    {
        BMP_PARSER_RETURN_ERROR("Error: Incorrect pixel array offset");
    }

    return EXIT_SUCCESS;
}

int bmp_create(BMPFile * bmp_file, const char* path)
{
    // obtain file's data using given target's api
    BMP_PARSER_OBTAIN_FILE_DATA(path);

    // file header has 14 bytes always + 40 from the BITMAPINFOHEADER
    // https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
    // https://docs.microsoft.com/pl-pl/previous-versions/dd183376(v=vs.85)
    if (file_size < 54)
    {
        free(file_content);
        BMP_PARSER_RETURN_ERROR("Error: BMP file must not be less than 54 bytes!");
    }

    // validate file header
    if (_validate_file_header(file_content, file_size))
    {
        free(file_content);
        BMP_PARSER_RETURN_ERROR("Error: File header structure is corrupted!");
    }

    // validate info header
    if (_validate_info_header(file_content, file_size))
    {
        free(file_content);
        BMP_PARSER_RETURN_ERROR("Error: Info header structure is corrupted!");
    }

    bmp_file->data = file_content;
    bmp_file->file_size = file_size;

    return EXIT_SUCCESS;
}

void bmp_destroy(BMPFile* bmp_file)
{
    free(bmp_file->data);
}

BMPFile * bmp_clone(BMPFile* bmp_file)
{
    BMPFile* new_bmp = (BMPFile*)malloc(sizeof(BMPFile));

    if (new_bmp == NULL)
    {
        printf("Unable to allocate memory");
        return NULL;
    }

    new_bmp->data = (uint8_t *)malloc(bmp_file->file_size);

    if (new_bmp->data == NULL)
    {
        printf("Unable to allocate memory");
        return NULL;
    }

    memcpy(new_bmp->data, bmp_file->data, bmp_file->file_size);
    new_bmp->file_size = bmp_file->file_size;

    return new_bmp;
}

void bmp_save(BMPFile* bmp_file, const char* file_name)
{
    FILE* fp = fopen(file_name, "wb");

    if (fp == NULL)
    {
        printf("Couldn't open file");
        return;
    }

    fwrite(bmp_file->data, bmp_file->file_size, 1, fp);
}

// prints file header based on names from the microsoft documentation
// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
void bmp_print_file_header(BMPFile* bmp_file)
{
    printf("BITMAPFILEHEADER:\n");
    
    uint16_t* dataU16bit = NULL;
    uint32_t* dataU32bit = NULL;

    BMP_PARSER_PRINT_UINT16("bfType", 0);
    BMP_PARSER_PRINT_UINT32("bfSize", 2);
    BMP_PARSER_PRINT_UINT16("bfReserved1", 6);
    BMP_PARSER_PRINT_UINT16("bfReserved2", 8);
    BMP_PARSER_PRINT_UINT32("bfOffBits", 10);
}

// prints info header based on names from the microsoft documentation
// https://docs.microsoft.com/pl-pl/previous-versions/dd183376(v=vs.85)
void bmp_print_info_header(BMPFile* bmp_file)
{
    printf("BITMAPINFOHEADER:\n");

    uint32_t* dataU32bit = NULL;
    uint16_t* dataU16bit = NULL;
    int32_t* data32bit = NULL;
    
    BMP_PARSER_PRINT_UINT32("biSize", 14);
    BMP_PARSER_PRINT_INT32("biWidth", 18);
    BMP_PARSER_PRINT_INT32("biHeight", 22);
    BMP_PARSER_PRINT_UINT16("biPlanes", 26);
    BMP_PARSER_PRINT_UINT16("biBitCount", 28);
    BMP_PARSER_PRINT_UINT32("biCompression", 30);
    BMP_PARSER_PRINT_UINT32("biSizeImage", 34);
    BMP_PARSER_PRINT_INT32("biXPelsPerMeter", 38);
    BMP_PARSER_PRINT_INT32("biYPelsPerMeter", 42);
    BMP_PARSER_PRINT_UINT32("biClrUsed", 46);
    BMP_PARSER_PRINT_UINT32("biClrImportant", 50);

    // TODO add support for more than 40bytes headers
}

// generate all file header's getters

BMP_PARSER_GENERATE_UINT16_GETTER(bfType, 0);
BMP_PARSER_GENERATE_UINT32_GETTER(bfSize, 2);
BMP_PARSER_GENERATE_UINT16_GETTER(bfReserved1, 6);
BMP_PARSER_GENERATE_UINT16_GETTER(bfReserved2, 8);
BMP_PARSER_GENERATE_UINT32_GETTER(bfOffBits, 10);

// generate all info header's getters
// TODO add support for more than 40bytes headers

BMP_PARSER_GENERATE_UINT32_GETTER(biSize, 14);
BMP_PARSER_GENERATE_INT32_GETTER(biWidth, 18);
BMP_PARSER_GENERATE_INT32_GETTER(biHeight, 22);
BMP_PARSER_GENERATE_UINT16_GETTER(biPlanes, 26);
BMP_PARSER_GENERATE_UINT16_GETTER(biBitCount, 28);
BMP_PARSER_GENERATE_UINT32_GETTER(biCompression, 30);
BMP_PARSER_GENERATE_UINT32_GETTER(biSizeImage, 34);
BMP_PARSER_GENERATE_INT32_GETTER(biXPelsPerMeter, 38);
BMP_PARSER_GENERATE_INT32_GETTER(biYPelsPerMeter, 42);
BMP_PARSER_GENERATE_UINT32_GETTER(biClrUsed, 46);
BMP_PARSER_GENERATE_UINT32_GETTER(biClrImportant, 50);

static inline uint32_t _calculate_byte_location(BMPFile* file, int32_t pixel_height, uint32_t pixel_width, uint8_t byte_index)
{
    // a pixel array start
    uint32_t pixel_array = *BMP_PARSER_CAST_TO_INT32_PTR(file->data[10]);

    int32_t height_pixels = *BMP_PARSER_CAST_TO_INT32_PTR(file->data[22]);
    int32_t width_pixels = *BMP_PARSER_CAST_TO_INT32_PTR(file->data[18]);

    // 3 bytes per pixel
    uint32_t width_bytes = (3 * width_pixels);

    uint8_t offset = width_bytes % 4;

    uint32_t width_real = width_bytes + offset;

    return pixel_array + pixel_height * width_real + pixel_width * 3 + byte_index;
}

// obtain pixel's byte
uint8_t get_pixel_byte(BMPFile* file, int32_t pixel_height, uint32_t pixel_width, uint8_t byte_index)
{
   return file->data[_calculate_byte_location(file, pixel_height, pixel_width, byte_index)];
}

void set_pixel_byte(BMPFile* file, int32_t pixel_height, uint32_t pixel_width, uint8_t byte_index, uint8_t byte)
{
    file->data[_calculate_byte_location(file, pixel_height, pixel_width, byte_index)] = byte;
}

// get RED color from given pixel
#if defined(BMP_GET_PIXEL_RED_COLOR)
#undef BMP_GET_PIXEL_RED_COLOR
#endif
#define BMP_GET_PIXEL_RED_COLOR(file, pixel_height, pixel_width) \
get_pixel_byte(file, pixel_height, pixel_width, 2)

// get GREEN color from given pixel
#if defined(BMP_GET_PIXEL_GREEN_COLOR)
#undef BMP_GET_PIXEL_GREEN_COLOR
#endif
#define BMP_GET_PIXEL_GREEN_COLOR(file, pixel_height, pixel_width) \
get_pixel_byte(file, pixel_height, pixel_width, 1)

// get BLUE color from given pixel
#if defined(BMP_GET_PIXEL_BLUE_COLOR)
#undef BMP_GET_PIXEL_BLUE_COLOR
#endif
#define BMP_GET_PIXEL_BLUE_COLOR(file, pixel_height, pixel_width) \
get_pixel_byte(file, pixel_height, pixel_width, 0)

// set RED color from given pixel to given value
#if defined(BMP_SET_PIXEL_RED_COLOR)
#undef BMP_SET_PIXEL_RED_COLOR
#endif
#define BMP_SET_PIXEL_RED_COLOR(file, pixel_height, pixel_width, value) \
set_pixel_byte(file, pixel_height, pixel_width, 2, value)

// set GREEM color from given pixel to given value
#if defined(BMP_SET_PIXEL_GREEN_COLOR)
#undef BMP_SET_PIXEL_GREEN_COLOR
#endif
#define BMP_SET_PIXEL_GREEN_COLOR(file, pixel_height, pixel_width, value) \
set_pixel_byte(file, pixel_height, pixel_width, 1, value)

// set BLUE color from given pixel to given value
#if defined(BMP_SET_PIXEL_BLUE_COLOR)
#undef BMP_SET_PIXEL_BLUE_COLOR
#endif
#define BMP_SET_PIXEL_BLUE_COLOR(file, pixel_height, pixel_width, value) \
set_pixel_byte(file, pixel_height, pixel_width, 0, value)

#endif

/* STEGANOGRAPHY */

#if !defined(PIXEL_MOVE)
#undef PIXEL_MOVE
#endif 
#define PIXEL_MOVE(COLUMN, ROW, WIDTH) \
++COLUMN; \
if(COLUMN == WIDTH) \
{ \
    COLUMN = 0; \
    ++ROW; \
}

#if !defined(GET_NTH_BIT_VALUE)
#undef GET_NTH_BIT_VALUE
#endif 
#define GET_NTH_BIT_VALUE(VARIABLE, INDEX) (VARIABLE >> INDEX) & 1

#if !defined(SET_NTH_BIT_VALUE_IF_TRUE)
#undef SET_NTH_BIT_VALUE_IF_TRUE
#endif 
#define SET_NTH_BIT_VALUE_IF_TRUE(VARIABLE, INDEX, CONDITION) if (CONDITION) { VARIABLE |= 1UL << INDEX; }


#if !defined(SET_LAST_BIT_ZERO)
#undef SET_LAST_BIT_ZERO
#endif 
#define SET_LAST_BIT_ZERO(VARIABLE) VARIABLE & ~1

#if !defined(SET_LAST_BIT_ONE)
#undef SET_LAST_BIT_ONE
#endif 
#define SET_LAST_BIT_ONE(VARIABLE) VARIABLE | 1

#if !defined(SET_NTH_BIT)
#undef SET_NTH_BIT
#endif
#define SET_NTH_BIT(INDEX, VARIABLE, BIT) GET_NTH_BIT_VALUE(VARIABLE, INDEX) ? SET_LAST_BIT_ONE(BIT) : SET_LAST_BIT_ZERO(BIT)


void encode_text(BMPFile* file, const char * text)
{
    if (bmp_get_biBitCount(file) == 24 && bmp_get_biCompression(file) == 0)
    {
        uint32_t text_size = strlen(text);
        uint32_t required_size = text_size + 4;

        uint32_t available_size = bmp_get_biHeight(file) * bmp_get_biWidth(file) * 3;

        if (required_size > available_size)
        {
            printf("Unable to write given text");
            return;
        }

        int8_t* to_encode = (int8_t*)malloc(required_size * sizeof(int8_t));

        if (to_encode == NULL)
        {
            printf("Unable to allocate memory");
            return;
        }

        memcpy(to_encode, &text_size, text_size * sizeof(int8_t));
        memcpy(&to_encode[4], text, text_size);

        int32_t height_pixels = bmp_get_biHeight(file);
        int32_t width_pixels = bmp_get_biWidth(file);

        int32_t row = 0;
        uint32_t column = 0;
        uint8_t bit = 0;

        for (uint32_t bytes = 0; bytes < required_size; bytes++)
        {
            bit = BMP_GET_PIXEL_BLUE_COLOR(file, row, column);
            BMP_SET_PIXEL_BLUE_COLOR(file, row, column, SET_NTH_BIT(0, to_encode[bytes], bit));

            bit = BMP_GET_PIXEL_GREEN_COLOR(file, row, column);
            BMP_SET_PIXEL_GREEN_COLOR(file, row, column, SET_NTH_BIT(1, to_encode[bytes], bit));

            bit = BMP_GET_PIXEL_RED_COLOR(file, row, column);
            BMP_SET_PIXEL_RED_COLOR(file, row, column, SET_NTH_BIT(2, to_encode[bytes], bit));

            PIXEL_MOVE(column, row, width_pixels);

            bit = BMP_GET_PIXEL_BLUE_COLOR(file, row, column);
            BMP_SET_PIXEL_BLUE_COLOR(file, row, column, SET_NTH_BIT(3, to_encode[bytes], bit));

            bit = BMP_GET_PIXEL_GREEN_COLOR(file, row, column);
            BMP_SET_PIXEL_GREEN_COLOR(file, row, column, SET_NTH_BIT(4, to_encode[bytes], bit));

            bit = BMP_GET_PIXEL_RED_COLOR(file, row, column);
            BMP_SET_PIXEL_RED_COLOR(file, row, column, SET_NTH_BIT(5, to_encode[bytes], bit));
   
            PIXEL_MOVE(column, row, width_pixels);

            bit = BMP_GET_PIXEL_BLUE_COLOR(file, row, column);
            BMP_SET_PIXEL_BLUE_COLOR(file, row, column, SET_NTH_BIT(6, to_encode[bytes], bit));

            bit = BMP_GET_PIXEL_GREEN_COLOR(file, row, column);
            BMP_SET_PIXEL_GREEN_COLOR(file, row, column, SET_NTH_BIT(7, to_encode[bytes], bit));

            PIXEL_MOVE(column, row, width_pixels);
        }
    }
    else
    {
        printf("Steganography can be perform only for uncompressed 24-bytes bitmaps");
    }
}

int8_t load_byte(BMPFile* file, int32_t * row, uint32_t * column)
{
    int32_t width_pixels = bmp_get_biWidth(file);
    int32_t row_buff = *row, column_buff = *column;
    int8_t byte = 0, bit = 0;
    
    bit = BMP_GET_PIXEL_BLUE_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 0, bit);

    bit = BMP_GET_PIXEL_GREEN_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 1, bit);

    bit = BMP_GET_PIXEL_RED_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 2, bit);
    
    PIXEL_MOVE(column_buff, row_buff, width_pixels);

    bit = BMP_GET_PIXEL_BLUE_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 3, bit);

    bit = BMP_GET_PIXEL_GREEN_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 4, bit);

    bit = BMP_GET_PIXEL_RED_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 5, bit);

    PIXEL_MOVE(column_buff, row_buff, width_pixels);

    bit = BMP_GET_PIXEL_BLUE_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 6, bit);

    bit = BMP_GET_PIXEL_GREEN_COLOR(file, row_buff, column_buff);
    bit = GET_NTH_BIT_VALUE(bit, 0);
    SET_NTH_BIT_VALUE_IF_TRUE(byte, 7, bit);

    PIXEL_MOVE(column_buff, row_buff, width_pixels);

    *row = row_buff;
    *column = column_buff;

    return byte;
}

void decode_text(BMPFile* file)
{
    if (bmp_get_biBitCount(file) == 24 && bmp_get_biCompression(file) == 0)
    {
        int8_t text_size[4];
        memset(text_size, 0, 4 * sizeof(int8_t));

        int32_t row = 0;
        uint32_t column = 0;

        text_size[0] = load_byte(file, &row, &column);
        text_size[1] = load_byte(file, &row, &column);
        text_size[2] = load_byte(file, &row, &column);
        text_size[3] = load_byte(file, &row, &column);

        int32_t* text_size_ptr = (int32_t *)&text_size;

        uint32_t available_size = bmp_get_biHeight(file) * bmp_get_biWidth(file) * 3;

        if (*text_size_ptr > available_size || !*text_size_ptr)
        {
            printf("Incorrect text size");
            return;
        }

        int8_t* text = (int8_t *)malloc((*text_size_ptr + 1) * sizeof(int8_t));

        if (text == NULL)
        {
            printf("Unable to allocate memory");
            return;
        }
        
        for (uint32_t i = 0; i < *text_size_ptr; i++)
        {
            text[i] = load_byte(file, &row, &column);
        }

        text[*text_size_ptr] = 0;

        printf("Steganography Message: '%s'", text);
    }
    else
    {
        printf("Steganography can be perform only for uncompressed 24-bytes bitmaps");
    }
}

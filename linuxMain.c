#include "BMP_HEADER.h"

int main(int argc, char** argv) {

    FILE* file;
    file = fileCreate(argv, 1, "rb");
    
    // obtain file size
    char buff[6];
    fgets(buff, 6, file);
    uint32_t* fsize = (uint32_t*)(buff+2);
    rewind(file);

    // validate file
    if (validateFile(buff)) { return EXIT_FAILURE; }

    // allocate memory for the file
    char* buffer;
    buffer = allocateBuffer(*fsize);
    
    // copy the content to the buffer
    size_t numberOfRead;
    numberOfRead = fread(buffer, 1,(*fsize), file);
    if (numberOfRead != *fsize) {
        printf("Reading error.");
    }

    // print data and histogram from requirements if 1 command line argument
    if (argc == 2) {
        printData(buffer);
        if (BICOMPRESSION == 0 && BIBITCOUNT == 24)
            histogramate(buffer);
        else
            printf("Histogram calculation is unsupported.");
    }

    //turn into grayscale if 2 command line arguments
    if (argc == 3) {
        FILE* resultFile;
        resultFile = fileCreate(argv, 2, "wb");
        if (BICOMPRESSION == 0 && BIBITCOUNT == 24)
            grayScaleMe(buffer, resultFile);
        else
            printf("Grayscale convertion is unsupported.");
        fileDestroy(resultFile);
    }
    fileDestroy(file);
    free(buffer);
    return EXIT_SUCCESS;
}

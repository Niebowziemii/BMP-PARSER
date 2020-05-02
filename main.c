#include "BMP_HEADER.h"
int main(int argc, char** argv) {

    // create an empty file
    FILE* file = create_a_file_from_name_stored_in_main_argument_list(argv, 1, "rb");
    
    // obtain file size
    char buff[6];
    fgets(buff, 6, file);
    uint32_t* fsize = (uint32_t*)(buff+2);
    rewind(file);

    // validate file
    if (validate_file(buff)) { return EXIT_FAILURE; }

    // allocate memory for the file
    char* buffer;
    buffer = (char*)malloc(*fsize * sizeof(char));
    if (buffer == NULL) {
        printf("Cannot allocate memory for file content.");
        return EXIT_FAILURE;
    }

    // copy the content to the buffer
    size_t num_of_bytes_in_the_file = fread(buffer, 1,(*fsize), file);
    if (num_of_bytes_in_the_file != *fsize) {
        printf("Unable to read a file.");
        return EXIT_FAILURE;
    }

    // print data and histogram from requirements if 1 command line argument
    if (argc == 2) {
        print_data(buffer);
        if (BICOMPRESSION(buffer) == 0 && BIBITCOUNT(buffer) == 24)
            generate_histogram(buffer);
        else
            printf("Histogram calculation is unsupported.");
    }

    //turn into grayscale if 2 command line arguments
    if (argc == 3) {
        FILE* result_file = create_a_file_from_name_stored_in_main_argument_list(argv, 2, "wb");
        if (result_file) {
            if (BICOMPRESSION(buffer) == 0 && BIBITCOUNT(buffer) == 24)
                turn_file_into_grayscale(buffer, result_file);
            else
                printf("Grayscale convertion is unsupported.");
            fclose(result_file);
        }
        else {
            printf("Result file cannot be created.");
            return EXIT_FAILURE;
        }
    }
    fclose(file);
    free(buffer);
    return EXIT_SUCCESS;
}

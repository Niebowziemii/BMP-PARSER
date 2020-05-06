#include "BMP_HEADER.h"
int main(int argc, char** argv) {
    if (argc > 1 && argc < 5) {
        // create an empty file
        FILE* file = create_a_file_from_name_stored_in_main_argument_list(argv, 1, "rb");
        if (file == NULL) {
            return EXIT_FAILURE;
        }
        struct stat buf;
        // obtain file size
        if ((fstat(fileno(file), &buf) != 0) || (!S_ISREG(buf.st_mode))) {
            fprintf(stderr, "[error]This will work only with regular file.");
            return EXIT_FAILURE;
        }
        off_t* fsize = &buf.st_size;

        // allocate memory for the file
        char* buffer;
        buffer = (char*)malloc(*fsize * sizeof(char));
        if (buffer == NULL) {
            printf("[error]Cannot allocate memory for file content.");
            return EXIT_FAILURE;
        }

        // copy the content to the buffer
        size_t read_characters = fread(buffer, 1, (*fsize), file);
        if (read_characters != *fsize) {
            printf("[error]Unable to read a file.");
            return EXIT_FAILURE;
        }
        if (validate_file(buffer)) { return EXIT_FAILURE; }
        // print data and histogram from requirements if 1 command line argument
        if (argc == 2) {
            print_data(buffer);
            if (BICOMPRESSION(buffer) == 0 && BIBITCOUNT(buffer) == 24) {
                generate_histogram(buffer);
            }
            else {
                printf("[error]Histogram calculation is unsupported.");
            }
        }

        //turn into grayscale if 2 command line arguments
        if (argc == 3) {
            FILE* result_file = create_a_file_from_name_stored_in_main_argument_list(argv, 2, "wb");
            if (result_file) {
                if (BICOMPRESSION(buffer) == 0 && BIBITCOUNT(buffer) == 24)
                    create_a_copy_of_given_file_in_grayscale(buffer, result_file);
                else
                    printf("[error]Grayscale convertion is unsupported.");
                fclose(result_file);
            }
            else {
                printf("[error]Result file cannot be created.");
                return EXIT_FAILURE;
            }
        }
        if (argc==4){
              FILE* result_file = create_a_file_from_name_stored_in_main_argument_list(argv, 2, "wb");
            if (result_file) {
                if (BICOMPRESSION(buffer) == 0 && BIBITCOUNT(buffer) == 24)
                    steganography(buffer, result_file,*fsize);
                else
                    printf("[error]Steganography convertion is unsupported.");
                fclose(result_file);
            }
            else {
                printf("[error]Result file cannot be created.");
                return EXIT_FAILURE;
            }
        }
        fclose(file);
        free(buffer);
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}

// project.c

#include "image_manip.h"
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return (exit) codes
#define RC_SUCCESS 0
#define RC_MISSING_FILENAME 1
#define RC_OPEN_FAILED 2
#define RC_INVALID_PPM 3
#define RC_INVALID_OPERATION 4
#define RC_INVALID_OP_ARGS 5
#define RC_OP_ARGS_RANGE_ERR 6
#define RC_WRITE_FAILED 7
#define RC_UNSPECIFIED_ERR 8

void print_usage(void);

int main(int argc, char* argv[])
{

    // base cli arguments
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    char operation[] = { '\0' };

    // if arguments is less than 4, then files are missing. 
    if (argc < 4) {
        return RC_MISSING_FILENAME;
    }

    // open input file 
    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        return RC_OPEN_FAILED;
    }

    Image image = read_ppm(input_file);
    if (image.data == NULL) {
        return RC_INVALID_PPM;
    }

    if (strcmp(argv[3], "pointilism") == 0) {
        if (argc != 4) {
            return RC_INVALID_OP_ARGS;
        }
    } else if (strcmp(argv[3], "rotate-ccw") == 0) {
        if (argc != 4) {
            return RC_INVALID_OP_ARGS;
        }
    } else if (strcmp(argv[3], "grayscale") == 0) {
        if (argc != 4) {
            return RC_INVALID_OP_ARGS;
        }
        image = grayscale(image);
    } else if (strcmp(argv[3], "blur") == 0) {
        if (argc != 5) {
            return RC_INVALID_OP_ARGS;
        }
        double sigma = atof(argv[4]);
        image = blur(image, sigma);
    } else if (strcmp(argv[3], "blend") == 0) {
        if (argc != 6) {
            return RC_INVALID_OP_ARGS;
        }
        char *input_filename2 = argv[2];
        FILE* input_file2 = fopen(input_filename2, "r");
        if (input_file2 == NULL) {
            return RC_OPEN_FAILED;
        }
        output_filename = argv[4];

        Image image2 = read_ppm(input_file2);
        if (image2.data == NULL) {
            return RC_INVALID_PPM;
        }
        double alpha = atof(argv[5]);
        image = blend(image, image2, alpha);

    } else if (strcmp(argv[3], "crop") == 0) {
        if (argc != 8) {
            return RC_INVALID_OP_ARGS;
        }
    }
    FILE* output_file = fopen(output_filename, "w");
    write_ppm(output_file, image);

    if(ferror(output_file)){
        return RC_WRITE_FAILED;
    }

    fclose(output_file);
    fclose(input_file);

    return RC_SUCCESS; // Replace Me!
}

void print_usage(void)
{
    printf("USAGE:\n");
    printf("   ./project <input-image> <output-image> grayscale\n");
    printf("   ./project <input-image> <output-image> rotate-ccw\n");
    printf("   ./project <input-image> <output-image> pointilism\n");
    printf("   ./project <input-image> <output-image> blur <sigma>\n");
    printf("   ./project <input-image> <output-image> crop <top left (x,y)> "
           "<bottom right (x,y)>\n");
    printf("   ./project <input-image1> <input-image2> blend <output-image> "
           "<alpha>\n");
}

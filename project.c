// project.c

#include "image_manip.h"
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    // if arguments is less than 4, then files are missing. 
    if (argc < 4) {
        fprintf(stderr, "Error: input filename, output filename, and operation are required\n");
        return RC_MISSING_FILENAME;
    }

    // base cli arguments
    char *input_filename = argv[1];
    char *output_filename = argv[2];

    // open input file 
    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: could not open input file '%s'\n", input_filename);
        return RC_OPEN_FAILED;
    }

    Image image = read_ppm(input_file);
    if (image.data == NULL) {
        fclose(input_file);
        return RC_INVALID_PPM;
    }

    if (strcmp(argv[3], "pointilism") == 0) {
        if (argc != 4) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: pointilism requires no additional arguments\n");
            return RC_INVALID_OP_ARGS;
        }
        Image image_t = pointilism(image, 1);
        free_image(&image);
        image = image_t;
    } else if (strcmp(argv[3], "rotate-ccw") == 0) {
        if (argc != 4) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: rotate-ccw requires no additional arguments\n");
            return RC_INVALID_OP_ARGS;
        }
        Image image_t = rotate_ccw(image);
        free_image(&image);
        image = image_t;
    } else if (strcmp(argv[3], "grayscale") == 0) {
        if (argc != 4) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: grayscale requires no additional arguments\n");
            return RC_INVALID_OP_ARGS;
        }
        image = grayscale(image);
    } else if (strcmp(argv[3], "blur") == 0) {
        if (argc != 5) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: blur requires exactly one sigma argument\n");
            return RC_INVALID_OP_ARGS;
        }
        double sigma = atof(argv[4]);
        if(!isfinite(sigma) || sigma <= 0){
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: sigma must be positive and finite\n");
            return RC_OP_ARGS_RANGE_ERR;
        }
        Image image_t = blur(image, sigma);
        free_image(&image);
        image = image_t;
    } else if (strcmp(argv[3], "blend") == 0) {
        if (argc != 6) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: blend requires an output filename and an alpha argument\n");
            return RC_INVALID_OP_ARGS;
        }
        char *input_filename2 = argv[2];
        FILE* input_file2 = fopen(input_filename2, "r");
        if (input_file2 == NULL) {
            fprintf(stderr, "Error: could not open second input file '%s'\n", input_filename2);
            fclose(input_file);
            free_image(&image);
            return RC_OPEN_FAILED;
        }
        output_filename = argv[4];

        Image image2 = read_ppm(input_file2);
        if (image2.data == NULL) {
            fclose(input_file);
            fclose(input_file2);
            free_image(&image);
            free_image(&image2);
            return RC_INVALID_PPM;
        }
        double alpha;
        if(sscanf(argv[5], "%lf", &alpha) != 1 || alpha < 0 || alpha > 1 || !isfinite(alpha)){
            fclose(input_file);
            fclose(input_file2);
            free_image(&image);
            free_image(&image2);
            fprintf(stderr, "Error: alpha must be between 0 and 1\n");
            return RC_OP_ARGS_RANGE_ERR;
        }
        Image image_t = blend(image, image2, alpha);
        free_image(&image);
        image = image_t;
        free_image(&image2);
        fclose(input_file2);

    } else if (strcmp(argv[3], "crop") == 0) {
        if (argc != 8) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: crop requires exactly four coordinates: left, top, right, bottom\n");
            return RC_INVALID_OP_ARGS;
        }
        Point top_left;
        Point bot_right;
        if (sscanf(argv[4], "%d", &top_left.x) != 1 || sscanf(argv[5], "%d", &top_left.y) != 1
            || sscanf(argv[6], "%d", &bot_right.x) != 1 || sscanf(argv[7], "%d", &bot_right.y) != 1) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: crop coordinates must be whole numbers\n");
            return RC_OP_ARGS_RANGE_ERR;
        }
        if (top_left.x < 0 || top_left.y < 0 || bot_right.x > image.cols || bot_right.y > image.rows
            || top_left.x >= bot_right.x || top_left.y >= bot_right.y) {
            fclose(input_file);
            free_image(&image);
            fprintf(stderr, "Error: crop corners are outside the image or out of order\n");
            return RC_OP_ARGS_RANGE_ERR;
        }
        Image image_t = crop(image, top_left, bot_right);
        free_image(&image);
        image = image_t;
    } else{
        fprintf(stderr, "Error: unsupported operation\n");
        free_image(&image);
        fclose(input_file);
        return RC_INVALID_OPERATION;
    }
    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Error: could not open output file '%s'\n", output_filename);
        fclose(input_file);
        free_image(&image);
        return RC_WRITE_FAILED;
    }

    int write_ok = write_ppm(output_file, image);
    int close_ok = fclose(output_file);
    fclose(input_file);
    free_image(&image);

    if (!write_ok || close_ok == EOF) {
        fprintf(stderr, "Error: could not write output file\n");
        return RC_WRITE_FAILED;
    }
    return RC_SUCCESS;
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

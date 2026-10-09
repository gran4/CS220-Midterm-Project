#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"


////////////////////////////////////////
// Definitions of the functions       //
// declared in image_manip.h go here! //
////////////////////////////////////////

Image grayscale( const Image in ){
    for(int i=0; i<in.rows*in.cols; i++){
        Pixel p = in.data[i];
        unsigned char gray_scale = (0.299 * p.r + 0.587*p.g + 0.114*p.b);
        printf(" %c ", gray_scale);
        in.data[i].r = gray_scale;
        in.data[i].g = gray_scale;
        in.data[i].b = gray_scale;
    }
    return in;
}




double *generate_gaussian(double sigma, int N){
    double *gaussian_matrix = malloc(N*N*sizeof(double));
    printf("%d ", N*N);

    for(int x=0; x<N; x++){
        for(int y=0; y<N; y++){
            int center = N / 2;
            int dx = x - center;
            int dy = y - center;
            double g = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));
            gaussian_matrix[x * N + y] = g;
        }
    }
    return gaussian_matrix;
}

void apply_gaussian(Image in, Image out, int row, int col, int N, double *gaussian_matrix){
    int center = N/2;

    double total_weight = 0;
    double r = 0;
    double g = 0;
    double b = 0;
    for(int x=0; x<N; x++){
        for(int y=0; y<N; y++){
            int neighbor_row = row + (x - center);
            int neighbor_col = col + (y - center);
            if(neighbor_row < 0 || neighbor_col < 0){
                continue ;
            }else if(neighbor_row >= in.rows || neighbor_col >= in.cols){
                continue ;
            }
            Pixel p2 = in.data[neighbor_row * in.cols + neighbor_col];
            double gaussian_multiplier = gaussian_matrix[x * N + y];
            total_weight += gaussian_multiplier;

            r += p2.r * gaussian_multiplier;
            g += p2.g * gaussian_multiplier;
            b += p2.b * gaussian_multiplier;
        }
    }
    int i = row*in.cols + col;
    out.data[i].r = r / total_weight;
    out.data[i].g = g / total_weight;
    out.data[i].b = b / total_weight;
}

Image blur( const Image in , double sigma ){
    int N = (int)ceil(sigma * 10);
    if (N % 2 == 0) {
        N++;
    }
    double *gaussian_matrix = generate_gaussian(sigma, N);
    
    Image out = make_image(in.rows, in.cols);

    for(int row=0; row<in.rows; row++){
        for(int col=0; col<in.cols; col++){
            apply_gaussian(in, out, row, col, N, gaussian_matrix);
        }
    }

    free(gaussian_matrix);
    return out;
}







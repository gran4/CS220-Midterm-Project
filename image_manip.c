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
        in.data[i].r = gray_scale;
        in.data[i].g = gray_scale;
        in.data[i].b = gray_scale;
    }
    return in;
}


Image blend( const Image in1, const Image in2 , double alpha ){
    int larger_rows = in1.rows;
    if(in2.rows > larger_rows){larger_rows = in2.rows; }
    int larger_cols = in1.cols;
    if(in2.cols > larger_cols){larger_cols = in2.cols; }

    Image output = make_image(larger_rows, larger_cols);

    for(int x=0; x<larger_rows; x++){
        for(int y=0; y<larger_cols; y++){
            int included_in1 = 0;
            int included_in2 = 0;
            Pixel p1;
            Pixel p2;
            if(x<in1.rows && y<in1.cols){
                p1 = in1.data[x*in1.cols + y];
                included_in1 = 1;
            }
            if(x<in2.rows && y<in2.cols){
                p2 = in2.data[x*in2.cols + y];
                included_in2 = 1;
            }
            if(included_in1 && included_in2){
                output.data[x*output.cols + y].r = p1.r*alpha + p2.r*(1-alpha);
                output.data[x*output.cols + y].g = p1.g*alpha + p2.g*(1-alpha);
                output.data[x*output.cols + y].b = p1.b*alpha + p2.b*(1-alpha);
            }else if(included_in1){
                output.data[x*output.cols + y].r = p1.r;
                output.data[x*output.cols + y].g = p1.g;
                output.data[x*output.cols + y].b = p1.b;
            }else if(included_in2){
                output.data[x*output.cols + y].r = p2.r;
                output.data[x*output.cols + y].g = p2.g;
                output.data[x*output.cols + y].b = p2.b;
            }else {
                output.data[x*output.cols + y].r = 0;
                output.data[x*output.cols + y].g = 0;
                output.data[x*output.cols + y].b = 0;
            }
        }
    }
    return output;
}






double *generate_gaussian(double sigma, int N){
    double *gaussian_matrix = malloc(N*N*sizeof(double));
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








Image crop( const Image in , Point top_left , Point bot_right ){
    int new_rows = bot_right.y - top_left.y;
    int new_cols = bot_right.x - top_left.x;
    Image out = make_image(new_rows, new_cols);
    return out;
}

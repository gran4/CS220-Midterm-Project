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



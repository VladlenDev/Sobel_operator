#include <stdlib.h>
#include <math.h>
#include "typeppm.h"

int kernelx[3][3] = {
	{-1, 0, 1}, 
	{-2, 0, 2}, 
	{-1, 0, 1}
};
int kernely[3][3] = {
	{-1, -2, -1}, 
	{ 0,  0,  0}, 
	{ 1,  2,  1}
};

void find_gradx(ppm_file *img, pixel *gradx) {
	int i, j;

	for(i = 1; i < img->ydim - 1; i++) {
    	for(j = 1; j < img->xdim - 1; j++) {

    		gradx[i*img->xdim+j].R = (
    			img->matrix[(i-1)*img->xdim+j-1].R * kernelx[2][2] + img->matrix[(i-1)*img->xdim+j].R * kernelx[2][1] + img->matrix[(i-1)*img->xdim+j+1].R * kernelx[2][0] + 
    			img->matrix[(i+0)*img->xdim+j-1].R * kernelx[1][2] + img->matrix[(i+0)*img->xdim+j].R * kernelx[1][1] + img->matrix[(i+0)*img->xdim+j+1].R * kernelx[1][0] + 
    			img->matrix[(i+1)*img->xdim+j-1].R * kernelx[0][2] + img->matrix[(i+1)*img->xdim+j].R * kernelx[0][1] + img->matrix[(i+1)*img->xdim+j+1].R * kernelx[0][0]
    		);
    		gradx[i*img->xdim+j].G = gradx[i*img->xdim+j].R;
    		gradx[i*img->xdim+j].B = gradx[i*img->xdim+j].R;
    	}
    }
}

void find_grady(ppm_file *img, pixel *grady) {
	int i, j;

	for(i = 1; i < img->ydim - 1; i++) {
    	for(j = 1; j < img->xdim - 1; j++) {

    		grady[i*img->xdim+j].R = (
    			img->matrix[(i-1)*img->xdim+j-1].R * kernely[2][2] + img->matrix[(i-1)*img->xdim+j].R * kernely[2][1] + img->matrix[(i-1)*img->xdim+j+1].R * kernely[2][0] + 
    			img->matrix[(i+0)*img->xdim+j-1].R * kernely[1][2] + img->matrix[(i+0)*img->xdim+j].R * kernely[1][1] + img->matrix[(i+0)*img->xdim+j+1].R * kernely[1][0] + 
    			img->matrix[(i+1)*img->xdim+j-1].R * kernely[0][2] + img->matrix[(i+1)*img->xdim+j].R * kernely[0][1] + img->matrix[(i+1)*img->xdim+j+1].R * kernely[0][0]
    		);
    		grady[i*img->xdim+j].G = grady[i*img->xdim+j].R;
    		grady[i*img->xdim+j].B = grady[i*img->xdim+j].R;
    	}
    }
}

void calculate_sobel(pixel *gradx, pixel *grady, pixel *sobel, int size) {
	int i, x, y;

	for(i = 0; i < size; i++) {
		x = gradx[i].R;
		y = grady[i].R;
		sobel[i].R = sqrt(x * x + y * y);
		sobel[i].G = sobel[i].R;
		sobel[i].B = sobel[i].R;
	}
}

void apply_gradx(ppm_file *img) {
	int size = img->xdim * img->ydim;
	pixel *gradx;

	gradx = (pixel*) malloc(size * sizeof(pixel));
	find_gradx(img, gradx);
	change_ppm_matrix(img, gradx);
	free(gradx);
}

void apply_grady(ppm_file *img) {
	int size = img->xdim * img->ydim;
	pixel *grady;

	grady = (pixel*) malloc(size * sizeof(pixel));
	find_gradx(img, grady);
	change_ppm_matrix(img, grady);
	free(grady);
}

void apply_sobel(ppm_file *img) {
	int size = img->xdim * img->ydim;
	pixel *gradx;
	pixel *grady;
	pixel *sobel;

	gradx = (pixel*) malloc(size * sizeof(pixel));
	find_gradx(img, gradx);
	
	grady = (pixel*) malloc(size * sizeof(pixel));
	find_gradx(img, grady);

	sobel = (pixel*) malloc(size * sizeof(pixel));
	calculate_sobel(gradx, grady, sobel, size);
	change_ppm_matrix(img, sobel);

	free(gradx);
	free(grady);
	free(sobel);
}
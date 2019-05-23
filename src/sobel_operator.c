#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "typeppm.h"

typedef struct gargs {
	int tnum;
	int threads;
	ppm_file *img;
	pixel *gradx;
	pixel *grady;
	pixel *sobel;
} gargs;

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

void *find_gradx(void *args) {
	gargs *gx = (gargs*) args;
	int i = 0, j, islast = 0, n = gx->threads, tnum = gx->tnum;
	pixel *gradx = gx->gradx;
	ppm_file *img = gx->img;

	if(tnum == 0)
		i = 1;

	if(tnum == n - 1)
		islast = 1;

	for(i = i + img->ydim / n * tnum; i < img->ydim / n * (tnum + 1) - islast; i++) {
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
    return 0;
}

void *find_grady(void *args) {
	gargs *gy = (gargs*) args;
	int i = 0, j, islast = 0, n = gy->threads, tnum = gy->tnum;
	pixel *grady = gy->grady;
	ppm_file *img = gy->img;

	if(tnum == 0)
		i = 1;

	if(tnum == n - 1)
		islast = 1;

	for(i = i + img->ydim / n * tnum; i < img->ydim / n * (tnum + 1) - islast; i++) {
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
    return 0;
}

void *calculate_sobel(void *args) {
	gargs *gs = (gargs*) args;
	int i, x, y, n = gs->threads, tnum = gs->tnum;
	pixel *sobel = gs->sobel;
	ppm_file *img = gs->img;
	int size = img->xdim * img->ydim;

	for(i = size / n * tnum; i < size / n * (tnum + 1); i++) {
		x = gs->gradx[i].R;
		y = gs->grady[i].R;
		sobel[i].R = sqrt(x * x + y * y);
		sobel[i].G = sobel[i].R;
		sobel[i].B = sobel[i].R;
	}
	return 0;
}

void apply_sobel(ppm_file *img, int n) {
	pthread_t threads[n];
	int i;
	int status_addr;
	int size = img->xdim * img->ydim;
	gargs sob[n];
	pixel *gradx;
	pixel *grady;
	pixel *sobel;

	gradx = (pixel*) malloc(size * sizeof(pixel));
	grady = (pixel*) malloc(size * sizeof(pixel));
	sobel = (pixel*) malloc(size * sizeof(pixel));

	for(i = 0; i < n; i++) {
		sob[i].tnum = i;
		sob[i].threads = n;
		sob[i].img = img;
		sob[i].gradx = gradx;
		sob[i].grady = grady;
		sob[i].sobel = sobel;
	}

	for(i = 0; i < n; i++)
		pthread_create(&threads[i], NULL, find_gradx, (void*)&sob[i]);

	for(i = 0; i < n; i++)
		pthread_join(threads[i], (void**)&status_addr);

	for(i = 0; i < n; i++)
		pthread_create(&threads[i], NULL, find_grady, (void*)&sob[i]);

	for(i = 0; i < n; i++)
		pthread_join(threads[i], (void**)&status_addr);

	for(i = 0; i < n; i++)
		pthread_create(&threads[i], NULL, calculate_sobel, (void*)&sob[i]);

	for(i = 0; i < n; i++)
		pthread_join(threads[i], (void**)&status_addr);

	change_ppm_matrix(img, sobel);

	free(gradx);
	free(grady);
	free(sobel);
}
#include <stdio.h>

#define MAXLINE 1024

typedef struct {
	int R, G, B;
} pixel;

typedef struct ppm_file {
	char *file_name;
	int is_ascii, xdim, ydim, colors;
	pixel *matrix;
} ppm_file;

void read_ppm_header(FILE *fp, ppm_file *img);
void init_ppm_matrix(ppm_file *img);
void read_ppm_data(FILE *fp, ppm_file *img);
ppm_file read_ppm_file(char *file_name);
void show_file_info(ppm_file *img);
void change_file_name(ppm_file *img, char *new_name);
void invert_colors(ppm_file *img);
void transform_color_to_gray(ppm_file *img);
void change_ppm_matrix(ppm_file *img, pixel *new_matrix);
void write_ppm_file(ppm_file *img);
void clear_file_info(ppm_file *img);

void *find_gradx(void *args);
void *find_grady(void *args);
void *calculate_sobel(void *args);
void apply_sobel(ppm_file *img, int n);
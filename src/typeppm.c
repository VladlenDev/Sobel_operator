#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "typeppm.h"

void read_ppm_header(FILE *fp, ppm_file *img) {
	int flag = 0;
	int x_val, y_val, maxcolors_val;
	int i;
	char magic[MAXLINE];
	char line[MAXLINE];
	int count = 0;
 
  	while(fgets(line, MAXLINE, fp) != NULL) {
    	flag = 0;

    	for(i = 0; i < strlen(line); i++) {
    		if(isgraph(line[i]) && (flag == 0)) {
        		if((line[i] == '#') && (flag == 0)) {
        			flag = 1;
        		}
    		}
    	}

    	if(flag == 0) {
    		if(count == 0)
        		count += sscanf(line, "%s %d %d %d", magic, &x_val, &y_val, &maxcolors_val); 
    		else if(count == 1)
        		count += sscanf(line, "%d %d %d", &x_val, &y_val, &maxcolors_val);
    		else if(count == 2)
        		count += sscanf(line, "%d %d", &y_val, &maxcolors_val);
        	else if(count == 3)
        		count += sscanf(line, "%d", &maxcolors_val);
    	}

    	if (count == 4)
    		break;
	}

	if(strcmp(magic, "P3") == 0)
    	img->is_ascii = 1;
	else if (strcmp(magic, "P6") == 0)
    	img->is_ascii = 0;
	else
    	img->is_ascii = -1;

  	img->xdim = x_val;
  	img->ydim = y_val;
  	img->colors = maxcolors_val;
}

void init_ppm_matrix(ppm_file *img) {
	int size = img->xdim * img->ydim;
	img->matrix = (pixel*) malloc(size * sizeof(pixel));
}

void read_ppm_data(FILE *fp, ppm_file *img) {
	int i = 0, c;
 	int r_val, g_val, b_val;
    
  	while((c = fgetc(fp)) != EOF) {
    	ungetc(c, fp);

    	if(img->is_ascii == 1) {
    		if(fscanf(fp, "%d %d %d", &r_val, &g_val, &b_val) != 3)
    			return;
    	}
    	else {
      		r_val = fgetc(fp);
      		g_val = fgetc(fp);
      		b_val = fgetc(fp);
    	}

    	img->matrix[i].R = r_val;
    	img->matrix[i].G = g_val;
    	img->matrix[i].B = b_val;
    	i++;
  	}
}

ppm_file read_ppm_file(char *file_name) {
	ppm_file img;
	FILE *fp;

	if((fp = fopen(file_name, "rb")) == NULL) {
		printf("%s reading failed! Check out the file!\n", file_name);
		abort();
	}

	img.file_name = file_name;
	read_ppm_header(fp, &img);
	init_ppm_matrix(&img);
	read_ppm_data(fp, &img);
	fclose(fp);
	return img;
}

void show_file_info(ppm_file *img) {

	if(img->matrix == NULL) {
		printf("\nWrong file or information is cleared\n\n");
		return;
	}

	printf("\nPath to file: %s\n", img->file_name);

	if(img->is_ascii == 0)
		printf("Magic number: P6\n");
	else
		printf("Magic number: P3\n");

	printf("Width: %d\nHeigh: %d\nClolor range: %d\n\n", img->xdim, img->ydim, img->colors);

}

void change_file_name(ppm_file *img, char *new_name) {
  img->file_name = new_name;
}

void invert_colors(ppm_file *img) {
  int size = img->xdim * img->ydim;

  for(int i = 0; i < size; i++) {
    img->matrix[i].R = img->colors - img->matrix[i].R;
    img->matrix[i].G = img->colors - img->matrix[i].G;
    img->matrix[i].B = img->colors - img->matrix[i].B;
  }
}

void transform_color_to_gray(ppm_file *img) {
	int mid_val = 0, size = img->xdim * img->ydim;

	for(int i = 0; i < size; i++) {
		mid_val = (img->matrix[i].R + img->matrix[i].G + img->matrix[i].B)/3;
		img->matrix[i].R = mid_val;
		img->matrix[i].G = mid_val;
		img->matrix[i].B = mid_val;
	}
}

void change_ppm_matrix(ppm_file *img, pixel *new_matrix) {
	int size = img->xdim * img->ydim;
	int i;

	for(i = 0; i < size; i++) {
		img->matrix[i].R = new_matrix[i].R;
		img->matrix[i].G = new_matrix[i].G;
		img->matrix[i].B = new_matrix[i].B;
	}
}

void write_ppm_file(ppm_file *img) {
	FILE *fp;
	int i, j;

	if((fp = fopen(img->file_name, "wb")) == NULL) {
		printf("%s writing failed! Check out the file!\n", img->file_name);
		abort();
	}

	if(img->is_ascii == 1)
    	fprintf(fp, "P3\n");
    else
    	fprintf(fp, "P6\n");

  	fprintf(fp, "# %s\n", img->file_name);
	fprintf(fp, "%d %d\n", img->xdim, img->ydim);
	fprintf(fp, "%d\n", img->colors);

  	for(i = 0; i < img->ydim; i++) {
    	for(j = 0; j < img->xdim; j++) {
      		if(img->is_ascii == 1) {
        		fprintf(fp, "%d %d %d ", 
          		img->matrix[i*img->xdim+j].R, 
          		img->matrix[i*img->xdim+j].G, 
          		img->matrix[i*img->xdim+j].B);
        		
        		if((j % 4) == 0)
          			fprintf(fp, "\n");
      		}
      		else {
        		fprintf(fp, "%c%c%c", 
          		img->matrix[i*img->xdim+j].R, 
          		img->matrix[i*img->xdim+j].G, 
          		img->matrix[i*img->xdim+j].B);
      		}
    	}
  	}

  	fclose(fp);
  	printf("File saved as %s\n", img->file_name);
}

void clear_file_info(ppm_file *img) {
	img->file_name = NULL;
	img->is_ascii = -1;
	img->xdim = -1;
	img->ydim = -1;
	img->colors = -1;
	free(img->matrix);
}
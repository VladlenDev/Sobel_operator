#include <string.h>
#include "typeppm.h"

void print_keys(void);

int main(int argc, char *argv[]) {
	char main_dir[256] = "/Users/user/Desktop/Images/";
	char *dir;
	int i, n = 1;

	if(argv[n] == NULL) {
		print_keys();
		return 0;
	}

	if(strcmp(argv[n], "-sd") == 0) {
		n++;
		dir = argv[n++];

		for(i = 0; dir[i] != '\0'; i++)
			main_dir[i] = dir[i];
		main_dir[i] = '\0';
	}

	if(strcmp(argv[n++],"-i") == 0) {

		char *file_in = argv[n];
		file_in = strcat(main_dir, file_in);
		ppm_file img_in = read_ppm_file(file_in);
		n++;

		for(; argv[n] != NULL && (strcmp(argv[n], "-gray") == 0 || strcmp(argv[n], "-inv") == 0 
			|| strcmp(argv[n], "-gdx") == 0 || strcmp(argv[n], "-sd") == 0 || strcmp(argv[n], "-gdy") == 0 
			|| strcmp(argv[n], "-sb") == 0 || strcmp(argv[n], "-inf") == 0); n++) {

			if(strcmp(argv[n], "-gray") == 0)
				transform_color_to_gray(&img_in);

			else if(strcmp(argv[n], "-inv") == 0)
				invert_colors(&img_in);

			else if(strcmp(argv[n], "-gdx") == 0) {
				transform_color_to_gray(&img_in);
				apply_gradx(&img_in);
			}

			else if(strcmp(argv[n], "-gdy") == 0) {
				transform_color_to_gray(&img_in);
				apply_grady(&img_in);
			}

			else if(strcmp(argv[n], "-sb") == 0) {
				transform_color_to_gray(&img_in);
				apply_sobel(&img_in);
			}

			else if(strcmp(argv[n], "-sd") == 0) {
				dir = argv[++n];

				for(i = 0; dir[i] != '\0'; i++)
					main_dir[i] = dir[i];
				main_dir[i] = '\0';
			}

			else if(strcmp(argv[n], "-inf") == 0)
				show_file_info(&img_in);
		}

		if(strcmp(argv[n++], "-o") == 0) {
			char *file_out = argv[n];

			for(i = 0; dir[i] != '\0'; i++)
				main_dir[i] = dir[i];

			main_dir[i] = '\0';
			file_out = strcat(main_dir, file_out);
			change_file_name(&img_in, file_out);
			write_ppm_file(&img_in);
		}
		else
			write_ppm_file(&img_in);

		clear_file_info(&img_in);
	}
	else
		print_keys();

	return 0;
}

void print_keys(void) {
	printf("\nUSAGE OF PPMREDACTOR+\n\n");
	printf("[-sd] ./ppmred+ -i \"FILENAME_IN\" [KEYS] -o \"FILENAME_OUT\"\n");
	printf("KEYS: \n[-gray] - turns RGB-colored ppm image to gray-scaled\n");
	printf("[-inv] - inverts colors of ppm image\n");
	printf("[-gdx] - applies gradient of x axis to ppm image\n");
	printf("[-gdy] - applies gradient of y axis to ppm image\n");
	printf("[-sb] - applies Sobel operator to ppm image\n");
	printf("[-sd] [/SAVE_DIRECTORY/] - sets up directory file be saved in (postfix) or opened from (infix)\n");
	printf("[-inf] - prints information about current image state\n\n");
}
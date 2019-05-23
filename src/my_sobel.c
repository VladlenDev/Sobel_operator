#include <string.h>
#include "typeppm.h"

void print_keys(void);

int main(int argc, char *argv[]) {
	char main_dir[256] = "/Users/user/Desktop/Images/";
	char *dir = NULL;
	int i, n = 1, last_slash, num_of_threads = 1;

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

	if(strcmp(argv[n++], "-i") == 0) {

		char *file_in = argv[n];
		file_in = strcat(main_dir, file_in);
		ppm_file img_in = read_ppm_file(file_in);
		n++;

		for(; argv[n] != NULL && (strcmp(argv[n], "-gray") == 0 || strcmp(argv[n], "-inv") == 0 
			|| strcmp(argv[n], "-sd") == 0 || strcmp(argv[n], "-sb") == 0 || strcmp(argv[n], "-inf") == 0); n++) {

			if(strcmp(argv[n], "-gray") == 0)
				transform_color_to_gray(&img_in);

			else if(strcmp(argv[n], "-inv") == 0)
				invert_colors(&img_in);

			else if(strcmp(argv[n], "-sb") == 0) {
				transform_color_to_gray(&img_in);

				if(strcmp(argv[n+1], "-thr") == 0) {
					n += 2;
					num_of_threads = 0;

					for(i = 0; argv[n][i] != '\0'; i++) {
						num_of_threads *= 10;
						num_of_threads += argv[n][i] - '0';
					}
				}

				apply_sobel(&img_in, num_of_threads);
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

			if(dir != NULL) {
				for(i = 0; dir[i] != '\0'; i++)
					main_dir[i] = dir[i];

				main_dir[i] = '\0';
			}

			for(i = 0; main_dir[i] != '\0'; i++)
				if(main_dir[i] == '/')
					last_slash = i;

			main_dir[++last_slash] = '\0';
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
	printf("./ppmred+ [-sd] -i \"FILENAME_IN\" [KEYS] -o \"FILENAME_OUT\"\n");
	printf("KEYS: \n[-gray] - turns RGB-colored ppm image to gray-scaled\n");
	printf("[-inv] - inverts colors of ppm image\n");
	printf("[-sb] - applies Sobel operator to ppm image. You can add [-thr] [\"NUM_OF_THREADS\"] to set number of threads for this algorythm\n");
	printf("[-sd] [/SAVE_DIRECTORY/] - sets up directory file be saved in (postfix) or opened from (infix)\n");
	printf("[-inf] - prints information about current image state\n\n");
}
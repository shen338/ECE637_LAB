

#include "ConnectSET.h"

void error(char *name);

int main(int argc, char **argv)
{

	FILE *fp;
	struct TIFF_img input_img, output_img;
	unsigned char **img;
	unsigned int **seg;
	int i, j;
	int ClassLabel = 1;
	int numCpixels;
	double T;
	struct pixel s;


	if (argc != 5) error(argv[0]);

	/* open image file */
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "cannot open file %s\n", argv[1]);
		exit(1);
	}

	/* read image */
	if (read_TIFF(fp, &input_img)) {
		fprintf(stderr, "error reading file %s\n", argv[1]);
		exit(1);
	}

	/* close image file */
	fclose(fp);

	/* check the type of image data */
	if (input_img.TIFF_type != 'g') {
		fprintf(stderr, "error: must be a grayscale image\n");
		exit(1);
	}

	/*get all parameters from input arguments*/
    s.x = strtod(argv[2], NULL);
	s.y = strtod(argv[3], NULL);
	T = strtod(argv[4], NULL);

	/*s.x = (int)argv[2];
	s.y = (int)argv[3];
	T = (int)argv[4];*/

	/*sscanf(argv[2], "%d", &(s.x));
	sscanf(argv[3], "%d", &(s.y));
	sscanf(argv[4], "%lf", &T);*/



	/* Allocate image of double precision floats */
	seg = (unsigned int **)get_img(input_img.width, input_img.height, sizeof(unsigned int));
	img = (unsigned char **)get_img(input_img.width, input_img.height, sizeof(unsigned char));

	/* copy img to double array */
	for (i = 0; i < input_img.height; i++)
		for (j = 0; j < input_img.width; j++) {
			img[i][j] = (unsigned char)input_img.mono[i][j];
		}

	/*get the connect area using given T, and pixel location*/ 
	connectSet(s, T, img, input_img.width, input_img.height, ClassLabel, seg, &numCpixels);



	/* set up structure for output achromatic image */
	/* to allocate a full color image use type 'c' */
	get_TIFF(&output_img, input_img.height, input_img.width, 'g');

	/*copy the result to output_img*/
	for (i = 0; i < input_img.height; i++) {
		for (j = 0; j < input_img.width; j++) {
			if (seg[i][j] == ClassLabel) {
				output_img.mono[i][j] = 0;
			}
			else
			{
				output_img.mono[i][j] = 255;
			}
		}
	}

	/* open output image file */
	if ((fp = fopen("output.tif", "wb")) == NULL) {
		fprintf(stderr, "cannot open file green.tif\n");
		exit(1);
	}

	/* write output image */
	if (write_TIFF(fp, &output_img)) {
		fprintf(stderr, "error writing TIFF file %s\n", argv[2]);
		exit(1);
	}

	/* close output image file */
	fclose(fp);

	/* de-allocate space which was used for the images */
	free_TIFF(&(input_img));

	free_img((void**)img);
	          
	return(0);
}
void error(char *name)
{
	printf("usage:  %s  image.tiff \n\n", name);
	exit(1);
}

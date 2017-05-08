

#include "ConnectSET.h"

void error(char *name);

int main(int argc, char **argv)
{

	FILE *fp;
	struct TIFF_img input_img, output_img;
	unsigned char **img;
	unsigned int **segment;
	int i, j;
	int ClassLabel = 1;
	int numCpixels;
	double T;
	struct pixel s;



	if (argc != 3) error(argv[0]);

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

	T = strtod(argv[2], NULL);

	/* Allocate image of double precision floats */
	segment = (unsigned int **)get_img(input_img.width, input_img.height, sizeof(unsigned int));
	img = (unsigned char **)get_img(input_img.width, input_img.height, sizeof(unsigned char));

	/* copy img to double array */
	for (i = 0; i < input_img.height; i++)
		for (j = 0; j < input_img.width; j++) {
			img[i][j] = (unsigned char)input_img.mono[i][j];
		}

	/*get the connect area of all pixels who do not belong to a connect set*/
	for (i = 0; i < input_img.height; i++)
		for (j = 0; j < input_img.width; j++) {
			if (segment[i][j] == 0)   // if already in a connect set, skip
			{
				s.x = i;
				s.y = j;
				connectSet(s, T, img, input_img.width, input_img.height, ClassLabel, segment, &numCpixels);
				if (numCpixels > 100) {   //  if only more than 100 pixels in a connectset, add ClassLabel.
					ClassLabel++;
					printf("%d\n", numCpixels);
				}
				else {
					connectSet(s, T, img, input_img.width, input_img.height, 0, segment, &numCpixels);
					// abanbon current connect set if num of pixel less than 100 
				}
			}
		}
	    ClassLabel--;
		printf("%d\n", ClassLabel);

	/* set up structure for output achromatic image */
	/* to allocate a full color image use type 'c' */
	get_TIFF(&output_img, input_img.height, input_img.width, 'g');

	/*copy the result to output_img*/
	for (i = 0; i < input_img.height; i++) {
		for (j = 0; j < input_img.width; j++) {
			output_img.mono[i][j] = segment[i][j];
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


#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

void error(char *name);

int main(int argc, char **argv)
{
	FILE *fp;
	struct TIFF_img input_img, filterd_img;
	double **img1, **img2, **img3, **oimg1, **oimg2, **oimg3;
	int32_t i, j;
	double pixel_r, pixel_g, pixel_b;

	if (argc != 2) error(argv[0]);

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
	if (input_img.TIFF_type != 'c') {
		fprintf(stderr, "error:  image must be 24-bit color\n");
		exit(1);
	}

	/* Allocate image of double precision floats */
	img1 = (double **)get_img(input_img.width, input_img.height, sizeof(double));
	img2 = (double **)get_img(input_img.width, input_img.height, sizeof(double));
	img3 = (double **)get_img(input_img.width, input_img.height, sizeof(double));
	oimg1 = (double **)get_img(input_img.width, input_img.height, sizeof(double));
	oimg2 = (double **)get_img(input_img.width, input_img.height, sizeof(double));
	oimg3 = (double **)get_img(input_img.width, input_img.height, sizeof(double));

	/* copy red/green/blue component to double array */
	for (i = 0; i < input_img.height; i++)
		for (j = 0; j < input_img.width; j++) {
			img1[i][j] = input_img.color[0][i][j];
			img2[i][j] = input_img.color[1][i][j];
			img3[i][j] = input_img.color[2][i][j];
		}


	/* Filter image with the IIR*/
	/*for (i = 0; i < input_img.height; i++) {
		oimg1[i][0] = img1[i][0];
		oimg2[i][0] = img2[i][0];
		oimg3[i][0] = img3[i][0];
	}

	for (j = 0; j < input_img.width; j++) {
		oimg1[0][j] = img1[0][j];
		oimg2[0][j] = img2[0][j];
		oimg3[0][j] = img3[0][j];
	}*/
	for (i = 0; i < input_img.height; i++) {
		for (j = 0; j < input_img.width; j++) {
			pixel_r = 0;
			pixel_g = 0;
			pixel_b = 0;

			pixel_r = pixel_r + 0.01*img1[i][j];
			pixel_g = pixel_g + 0.01*img2[i][j];
			pixel_b = pixel_b + 0.01*img3[i][j];

			if (i > 1 && j > 1) {
				pixel_r = pixel_r - 0.81*oimg1[i - 1][j - 1];
				pixel_g = pixel_g - 0.81*oimg2[i - 1][j - 1];
				pixel_b = pixel_b - 0.81*oimg3[i - 1][j - 1];
			}
			if (j > 1) {
				pixel_r = pixel_r + 0.9*oimg1[i][j - 1];
				pixel_g = pixel_g + 0.9*oimg2[i][j - 1];
				pixel_b = pixel_b + 0.9*oimg3[i][j - 1];
			}
			if (i > 1) {
				pixel_r = pixel_r + 0.9*oimg1[i - 1][j];
				pixel_g = pixel_g + 0.9*oimg2[i - 1][j];
				pixel_b = pixel_b + 0.9*oimg3[i - 1][j];
			}


			/*if (i > 0) {
				pixel_r = pixel_r + 0.9*oimg1[i - 1][j];
				pixel_g = pixel_g + 0.9*oimg2[i - 1][j];
				pixel_b = pixel_b + 0.9*oimg3[i - 1][j];
			}
			if (j > 0) {
				pixel_r = pixel_r + 0.9*oimg1[i][j-1];
				pixel_g = pixel_g + 0.9*oimg2[i][j-1];
				pixel_b = pixel_b + 0.9*oimg3[i][j-1];
			}
			if (i > 1 && j > 1) {
				pixel_r = pixel_r - 0.81*oimg1[i-1][j - 1];
				pixel_g = pixel_g - 0.81*oimg2[i-1][j - 1];
				pixel_b = pixel_b - 0.81*oimg3[i-1][j - 1];
			}*/
			oimg1[i][j] = pixel_r;
			oimg2[i][j] = pixel_g;
			oimg3[i][j] = pixel_b;
		}
	}



	

	/* set up structure for output color image */
	/* Note that the type is 'c' rather than 'g' */
	get_TIFF(&filterd_img, input_img.height, input_img.width, 'c');


	/* Illustration: constructing a sample color image -- interchanging the red and green components from the input color image */
	for (i = 0; i < input_img.height; i++)
		for (j = 0; j < input_img.width; j++) {
			if (oimg1[i][j] > 255)
			{
				oimg1[i][j] = 255;
			}
			if (oimg1[i][j] < 0)
			{
				oimg1[i][j] = 0;
			}
			if (oimg2[i][j] > 255)
			{
				oimg2[i][j] = 255;
			}
			if (oimg2[i][j] < 0)
			{
				oimg2[i][j] = 0;
			}
			if (oimg3[i][j] > 255)
			{
				oimg3[i][j] = 255;
			}
			if (oimg3[i][j] < 0)
			{
				oimg3[i][j] = 0;
			}
			filterd_img.color[0][i][j] = oimg1[i][j];
			filterd_img.color[1][i][j] = oimg2[i][j];
			filterd_img.color[2][i][j] = oimg3[i][j];
		}

	/* open color image file */
	if ((fp = fopen("IIR.tif", "wb")) == NULL) {
		fprintf(stderr, "cannot open file color.tif\n");
		exit(1);
	}

	/* write color image */
	if (write_TIFF(fp, &filterd_img)) {
		fprintf(stderr, "error writing TIFF file %s\n", argv[2]);
		exit(1);
	}

	/* close color image file */
	fclose(fp);

	/* de-allocate space which was used for the images */
	free_TIFF(&(input_img));

	free_TIFF(&(filterd_img));

	free_img((void**)img1);
	free_img((void**)img2);
	free_img((void**)img3);
	free_img((void**)oimg1);
	free_img((void**)oimg2);
	free_img((void**)oimg3);

	return(0);
}

void error(char *name)
{
	printf("usage:  %s  image.tiff \n\n", name);
	printf("this program reads in a 24-bit color TIFF image.\n");
	printf("It then horizontally filters the green component, adds noise,\n");
	printf("and writes out the result as an 8-bit image\n");
	printf("with the name 'green.tiff'.\n");
	printf("It also generates an 8-bit color image,\n");
	printf("that swaps red and green components from the input image");
	exit(1);
}



#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

void error(char *name);
int filter(struct TIFF_img input, int x, int y) {
	unsigned int image[25];
	unsigned int filter[25] = {1,1,1,1,1,1,2,2,2,1,1,2,2,2,1,1,2,2,2,1,1,1,1,1,1 };
	unsigned int temp;
	unsigned int num = 0;
	int location;
	for (int i = x - 2; i < x + 3; i++) {
		for (int j = y - 2; j < y + 3; j++) {
			image[num] = input.mono[i][j];
			num++;
		}
	}
	for (int i = 0; i < 24; i++) {
		int isSorted = 1;
			for (int j = 0; j<24 - i; j++)
			{
				if (image[j] > image[j + 1])
				{
					isSorted = 0;
					temp = image[j];
					image[j] = image[j + 1];
					image[j + 1] = temp;

					temp = filter[j];
					filter[j] = filter[j + 1];
					filter[j + 1] = temp;
				}
			}
			if (isSorted == 1) break; 
		}
	temp = 0;
	for (int i = 0; i < 25; i++) {
		temp = temp + filter[i];
		if (temp >= 17) {
			location = i;
			break;
		}
	}
	return image[location];
}
int main (int argc, char **argv) 
{
  FILE *fp;
  struct TIFF_img input_img, output_img;
  double **img1,**img2;
  int i,j;

  if ( argc != 2 ) error( argv[0] );

  /* open image file */
  if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file %s\n", argv[1] );
    exit ( 1 );
  }

  /* read image */
  if ( read_TIFF ( fp, &input_img ) ) {
    fprintf ( stderr, "error reading file %s\n", argv[1] );
    exit ( 1 );
  }

  /* close image file */
  fclose ( fp );

  /* check the type of image data */
  if ( input_img.TIFF_type != 'g' ) {
    fprintf ( stderr, "error:  image must be grayscale\n" );
    exit ( 1 );
  }

  /* Allocate image of double precision floats */
  img1 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));

  /* copy image to double array */
  for ( i = 0; i < input_img.height; i++ )
  for ( j = 0; j < input_img.width; j++ ) {
       img1[i][j] = input_img.mono[i][j];
  }

  /* Filter image */
  for ( i = 2; i < input_img.height - 2; i++ )
  for ( j = 2; j < input_img.width - 2; j++ ) {
	  img2[i][j] = filter(input_img, i, j);
  }

  /* set up structure for output image */
  get_TIFF(&output_img, input_img.height, input_img.width, 'g');

  /* Fill in boundary pixels */
  for (i = 0; i < input_img.height; i++)
	  for (j = 0; j < input_img.width; j++) {
		  if (i<2||i>input_img.height-3||j<2||j> input_img.width - 3)
		  {
			  img2[i][j] = 0;
		  }
		  output_img.mono[i][j] = img2[i][j];
	  }



  /* open output image file */
  if ( ( fp = fopen ( "output_img.tif", "wb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file green.tif\n");
    exit ( 1 );
  }

  /* write output image */
  if ( write_TIFF ( fp, &output_img ) ) {
    fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
    exit ( 1 );
  }

  /* close green image file */
  fclose ( fp );
    
  /* de-allocate space which was used for the images */
  free_TIFF ( &(input_img) );
  free_TIFF ( &(output_img) );
  
  free_img( (void**)img1 );
  free_img( (void**)img2 );  

  return(0);
}

void error(char *name)
{
    exit(1);
}


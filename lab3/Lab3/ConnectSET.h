#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

struct pixel {
	int x;
	int y;
};
struct Node{
	struct pixel current;
	struct Node *next;
};
void connectNeighbors(
	struct pixel s,
	double T,
	unsigned char **img,
	int width,
	int height,
	int *M,
	struct pixel c[4]);
void connectSet(
	struct pixel s,
	double T,
	unsigned char **img,
	int width,
	int height,
	int ClassLabel,
	unsigned int **seg,
	int *numCpixels);
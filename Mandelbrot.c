/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
	u_int64_t iters = 0;
	struct ComplexNumber *complexZ = newComplexNumber(0, 0);
	
	while (maxiters > 0) {
		iters += 1;
		struct ComplexNumber *pointcopy = newComplexNumber(Re(point), Im(point));
		struct ComplexNumber *complexY = newComplexNumber(Re(complexZ), Im(complexZ));
		complexZ = ComplexProduct(complexZ, complexY);
		complexZ = ComplexSum(complexZ, pointcopy);
		if (ComplexAbs(complexZ) > threshold) {
			freeComplexNumber(complexZ);
			return iters;
		}
		maxiters -= 1;
	}
	freeComplexNumber(complexZ);
	return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output) {
	double size = ((2 * resolution) + 1) * ((2 * resolution) + 1);
	double colrow = ((2 * resolution) + 1);
	double scalefactor = 1/((colrow - 1)/2);
	double centerCoord = (scale - 1)/2;
	double realCenter = Re(center);
	double imCenter = Im(center);

	int index = 0;
	while (index < size) {
		double width = index % (int) scale;
		if (width > centerCoord) {
			width *= -1;
		}
		double height = 0;
		double counter = scale;
		while (index < counter) {
			counter += scale;
			height + 1;
		}
		double real = realCenter - (scale * scalefactor * width);
		double imaginary = imCenter - (scale * scalefactor * height);
		struct ComplexNumber *newPoint = newComplexNumber(real, imaginary);
		output[index] = MandelbrotIterations(max_iterations, newPoint, threshold);
		index += 1;
	}
	freeComplexNumber(center);
}



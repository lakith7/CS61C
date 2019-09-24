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
	struct ComplexNumber *complexY;
	struct ComplexNumber *complexX = newComplexNumber(0, 0);

	while (maxiters >= 0) {
		if (ComplexAbs(complexX) > threshold) {
			freeComplexNumber(complexX);
			return iters;
		}
		complexY = ComplexProduct(complexX, complexX);
		freeComplexNumber(complexX);
		complexX = ComplexSum(complexY, point);
		freeComplexNumber(complexY);
		maxiters -= 1;
		iters += 1;
	}
	freeComplexNumber(complexX);
	return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output) {
	double realCenter = Re(center);
	double imCenter = Im(center);
	double increments = scale/resolution;
	double imaginary;
	double real;

	int length = (2 * resolution + 1);
	int height = 0;
	int width = 0;
	int index = 0;
	while (height < length) {
		imaginary = imCenter + scale - (increments * height);
		while (width < length) {
			real = realCenter - scale + (increments * width);
			ComplexNumber *newPoint = newComplexNumber(real, imaginary);
			output[index] = MandelbrotIterations(max_iterations, newPoint, threshold);
			freeComplexNumber(newPoint);
			index += 1;
			width += 1;
		}
		height += 1;
		width = 0;
	}
	freeComplexNumber(center);
}



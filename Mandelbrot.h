/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>

/*
This function returns the number of iterations that cause the initial point to exceed the threshold.
If the threshold is not exceeded after maxiters, the function returns maxiters.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold) {
	u_int64_t iters = 0;
	struct ComplexNumber *complexZ = newComplexNumber(0, 0);
	while (maxiters > 0) {
		iters += 1;
		complexZ = ComplexProduct(complexZ, complexZ);
		complexZ = ComplexSum(complexZ, point);
		if (ComplexAbs(complexZ) > threshold) {
			freeComplexNumber(complexZ);
			freeComplexNumber(point);
			return iters;
		}
		maxiters -= 1;
	}
	freeComplexNumber(complexZ);
	freeComplexNumber(point);
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
	double center = (scale - 1)/2;
	double realCenter = Re(center);
	double imCenter = Im(center);

	double index = 0;
	while (index < size) {
		double width = index % scale;
		if (width > center) {
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
}

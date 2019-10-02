/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>
#include <string.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    /* Output is given as an argument, so make sure to malloc the correct amount of space before inputting it into the function. Malloc only the 2d array, the 1d part is done below */
    double counter = 0;
    double scale;
    for (int index = 0; index < framecount; index += 1) {
    	output[index] = malloc(sizeof(u_int64_t) * ((2 * resolution) + 1) * ((2 * resolution) + 1));
    	scale = initialscale * (pow((finalscale/initialscale), (counter/(((double) framecount) - 1))));
    	/* framecount cannot be 0 due to checking before it is inputted into this function. */
    	Mandelbrot(threshold, max_iterations, center, scale, resolution, output[index]);
    	counter += 1;
    }

}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
	}
	double threshold, initialscale, finalscale;
	int framecount;
	ComplexNumber* center;
	u_int64_t max_iterations, resolution;

	threshold = atof(argv[1]);
	max_iterations = (u_int64_t)atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	initialscale = atof(argv[5]);
	finalscale = atof(argv[6]);
	framecount = atoi(argv[7]);
	resolution = (u_int64_t)atoi(argv[8]);
	char* output_folder = argv[9];
	char* colorfile = argv[10];
	if (threshold <= 0 || finalscale <= 0 || initialscale <= 0 || max_iterations <= 0 || resolution <= 0) {
		printf("threshold, finalscale, initalscale, resolution, and max_iterations must be greater than 0");
		return 1;
	}
	if (framecount > 10000 || framecount <= 0) {
		printf("invalid framecount input");
		return 1;
	}
	if ((framecount == 1) && (initialscale != finalscale)) {
		printf("invalid framecount and initialscale/finalscale input");
		return 1;
	}

	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	int* colorcount = malloc(sizeof(int));
	if (colorcount == NULL) {
		printf("memory allocation problems");
		free(colorcount);
		return 1;
	}
	uint8_t** colorMap = FileToColorMap(colorfile, colorcount);

	u_int64_t** output = (u_int64_t**) malloc(framecount * sizeof(u_int64_t*));

	if (output == NULL) {
		printf("memory allocation problems");
		freeDoublePointer(colorMap);
		free(output);
		free(colorcount);
		return 1;
	}

	MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, output);


	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/




	uint8_t* color;
	int frameNumber = 0;
	int indexer;




	for (int i = 0; i < framecount; i++) {
		char* fileName = (char*) malloc((sizeof(char)) * (16 + (strlen(output_folder))));
		sprintf(fileName, "%s/frame%05d.ppm", output_folder, frameNumber);
		frameNumber += 1;
		FILE* fileptr = fopen(fileName, "w");
		fprintf(fileptr, "P6 %lu %lu 255\n", ((2 * resolution) + 1), ((2 * resolution) + 1)); 
		uint8_t* outputList = (uint8_t*) malloc(3 * ((2*resolution)+1) * ((2*resolution)+1) * sizeof(uint8_t));
		int index = 0;
		if (outputList == NULL) {
			free(outputList);
			fclose(fileptr);
			free(fileName);
			return 1;
		}

		for (int y = 0; y < ((2 * resolution + 1) * (2 * resolution + 1)); y++) {
					if (output[i][y] == 0) {
						outputList[index] = (uint8_t) 0;
						index += 1;
						outputList[index] = (uint8_t) 0;
						index += 1;
						outputList[index] = (uint8_t) 0;
						index += 1;
					} else {
						/* Pretty sure modulus has to be between two ints, so cast if needed. */
						indexer = (((output[i][y]) % *colorcount) - 1) % *colorcount;
						color = colorMap[indexer];
						outputList[index] = (uint8_t) color[0];
						index += 1;
						outputList[index] = (uint8_t) color[1];
						index += 1;
						outputList[index] = (uint8_t) color[2];
						index += 1;
					}

		}
		/*
		for (int j = 0; j < 3 * ((2*resolution)+1) * ((2*resolution)+1); j++) {
			fprintf(fileptr, "ActualColorNumber: %d\n", outputList[j]);
		} */
		fwrite(outputList, sizeof(uint8_t), (3 * (2 * resolution + 1) * (2 * resolution + 1)), fileptr);
		free(fileName);
		fclose(fileptr);
		free(outputList);
	}



	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	freeComplexNumber(center);
	freeDoublePointer(colorMap, colorcount);
	free(colorcount);
	for (int q = 0; q < framecount; q++) {
		free(output[q]);
	}
	free(output);

	return 0;
}




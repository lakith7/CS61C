/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount) {
	FILE* fileptr = fopen(colorfile, "r");

	if (fileptr == NULL) {
		fclose(fileptr);
		printf("No such file.");
		return 0;
	}

	fscanf(fileptr, "%u", colorcount);

	uint8_t* storage1 = (uint8_t*) malloc(sizeof(uint8_t)*10);
	uint8_t* storage2 = (uint8_t*) malloc(sizeof(uint8_t)*10);
	uint8_t* storage3 = (uint8_t*) malloc(sizeof(uint8_t)*10);
	if (storage1 == NULL || storage2 == NULL || storage3 == NULL) {
		printf("memory allocation problems");
		free(storage1);
		free(storage2);
		free(storage3);
		fclose(fileptr);
		return 0;
	}

	if ((*colorcount) <= 0) {
		fclose(fileptr);
		printf("colorcount argument is at or below zero");
		free(storage1);
		free(storage2);
		free(storage3);
		return 0;
	}

	uint8_t** returnPointer = (uint8_t**) malloc((*colorcount) * sizeof(uint8_t*));
	if (returnPointer == NULL) {
		printf("memory allocation problems");
		free(storage1);
		free(storage2);
		free(storage3);
		fclose(fileptr);
		free(returnPointer);
		return 0;
	}
	for (int i = 0; i < (*colorcount); i++) {
		returnPointer[i] = (uint8_t*) malloc(3 * sizeof(uint8_t));
		if (returnPointer[i] == NULL) {
			printf("memory allocation problems");
			while (i >= 0) {
				free(returnPointer[i]);
				i -= 1;
			}
			fclose(fileptr);
			free(returnPointer);
			free(storage1);
			free(storage2);
			free(storage3);
			return 0;
		}
	}
	/* Do we have to malloc the null character or nah? */
	int index = 0;
	int numberColors = *colorcount;
	printf("%d", numberColors);
	while (numberColors > 0) {
		if (fscanf(fileptr, "%u %u %u", storage1, storage2, storage3) != 3) {
			printf("the input file has problematic formatting");
			freeDoublePointer(returnPointer, colorcount);
			free(storage1);
			free(storage2);
			free(storage3);
			fclose(fileptr);
			return 0;
		}
		returnPointer[index][0] = *storage1;
		returnPointer[index][1] = *storage2;
		returnPointer[index][2] = *storage3;
		/* Do we have to malloc the below commands or nah? */
        numberColors -= 1;
        index += 1;
	}
	free(storage1);
	free(storage2);
	free(storage3);
    fclose(fileptr);
    return returnPointer;
}

void freeDoublePointer(uint8_t** input, int* colorcount) {
	int numberOfColors = *colorcount;
	int index = 0;
	while (index < numberOfColors) {
		free(input[index]);
		index += 1;
	}
	free(input);
}



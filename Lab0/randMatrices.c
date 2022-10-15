#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

/*  rxc matrix the elements are filled with random numbers
	between 0 to k.
	outputfile is the random matrix.
	histogram test?
*/


int main(int argc, char *argv[])
{
	//command line arguments start

	if( argc != 5 ) 
	{
		printf("Invaild number of Arguments entered \n");
		return -1;
	}

	int k, c, r;

	//convert string to integer values
	r = atoi(argv[1]);
	c = atoi(argv[2]);
	k = atoi(argv[3]);

	printf("r:%d c:%d k:%d\n", r, c, k);

	
	//inputs range check the assignment
	if(!(r > 0 && r <= 10000) || !(c > 0 && c <= 10000) || !(k >= 2 && k <= MIN(r, c)/10))
	{
		printf("inputs not in range\n");
		return -1;
	}

	//end of command line arguments

	srand( time(NULL) );

	//function prototypes

	void fill_random(int **, int , int , int );

	void writeToFile(FILE*, int**, int, int);
	//file handling
	FILE *outFPtr = fopen(argv[4], "w");

	//to calculate executiontime
	struct timeval start, end;
	//start of execution
	gettimeofday(&start, NULL);

	//allocating space for the matrix
	int **matrix = (int**)malloc(c*sizeof(int*));
		for(int i = 0; i < r; i++)
			matrix[i] = malloc(sizeof(int)*c);

	fill_random(matrix,r,c,k);
	writeToFile(outFPtr,matrix, r, c);

	//end of execution
	gettimeofday(&end, NULL);

	//free matrix to prevent memory leaks?
	for (int i = 0; i < r; i++)
	free(matrix[i]);
	free(matrix);

	//execution time calculation

	float exec_time = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	exec_time /= 1000000;
	printf("Time taken to execute: %fs\n\n", exec_time);

	return 0;
	
}


//functions

void fill_random(int **matrix, int r, int c, int k)
{
	//using rand to fill every element of the matrix
	for (int i = 0; i < r; i++)
		for(int j = 0; j < c; j++)
			matrix[i][j] = (rand() % k) + 1;
}

void writeToFile(FILE *outFPtr, int **matrix, int r, int c)
{
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
			fprintf(outFPtr, "%d ", matrix[i][j]);

		fprintf(outFPtr, "\n");
	}
}
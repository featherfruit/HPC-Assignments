#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		printf("Invaild number of Arguments entered \n");
		return -1;
	}

	int k, c, r,num_thread;
	r = atoi(argv[1]);
	c = atoi(argv[2]);
	k = atoi(argv[3]);
	num_thread = atoi(argv[5]);

	if (!(r > 0 && r <= 100000) || !(c > 0 && c <= 100000) || !(k >= 2 && k <= MIN(r, c) / 10))
	{
		printf("inputs not in range\n");
		return -1;
	}

	srand(time(NULL));

	void fill_random(int **, int, int, int, int);
	void writeToFile(FILE *, int **, int, int);

	// to calculate executiontime
	struct timeval start, end;
	// start of execution
	gettimeofday(&start, NULL);

	FILE *outFPtr = fopen(argv[4], "w");

	int **matrix = (int **)malloc(r * sizeof(int *));
	for (int i = 0; i < r; i++)
		matrix[i] = malloc(sizeof(int) * c);

	fill_random(matrix, r, c, k, num_thread);
	writeToFile(outFPtr, matrix, r, c);

	// end of execution
	gettimeofday(&end, NULL);

	for (int i = 0; i < r; i++)
		free(matrix[i]);
	free(matrix);

	float exec_time = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	exec_time /= 1000000;
	printf("Time taken to execute: %fs\n\n", exec_time);

	return 0;
}

void fill_random(int **matrix, int r, int c, int k, int num_thread)
{
	int seed1;
	int i,j;

// loop directive contains a collapse clause as there is more than one associated loop
omp_set_dynamic(0); // Explicitly disable dynamic teams
omp_set_num_threads(num_thread);
#pragma omp parallel
	{
	// thread 1
	rand_r(&seed1);
#pragma omp parallel for  shared(matrix) private(i,j,seed1)
		for (int i = 0; i < r; i++)
			for (int j = 0; j < c; j++)
				matrix[i][j] = ((rand_r(&seed1) % k) + 1);
	}
}

void writeToFile(FILE *outFPtr, int **matrix, int r, int c)
{
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			fprintf(outFPtr, "%d ", matrix[i][j]);

		fprintf(outFPtr, "\n");
	}
	printf("\n");
}
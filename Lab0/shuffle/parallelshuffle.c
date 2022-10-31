/*
	Author:Featherfruit on Github
	Program to shuffle a r x c matrix
	the shuffle is based on random permutation of
	rows or columns.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>


int main(int argc, char *argv[])
{
	if (argc != 7)
	{
		printf("Invaild  number of Arguments entered \n");
		return -1;
	}

	int r, c,num_thread;
	r = atoi(argv[1]);
	c = atoi(argv[2]);
	num_thread = atoi(argv[6]);

	srand(time(NULL));
	void shufflerc(int **, int, int, char, int, int,int);
	void writeToFile(FILE *, int **, int, int);

	// to calculate executiontime
	struct timeval start, end;
	// start of execution
	//gettimeofday(&start, NULL);

	int **matrix = (int **)malloc(r * sizeof(int *));
	for (int i = 0; i < r; i++)
		matrix[i] = malloc(sizeof(int) * c);

	FILE *fp = fopen(argv[3], "r");

	if (fp == NULL)
		return 0;

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			fscanf(fp, "%d", &matrix[i][j]);
	}
	fclose(fp);

	fp = fopen(argv[4], "r");

	if (fp == NULL)
	{
		printf("fp was null case use a file with commands");
		return 0;
	}
	char rcidentifier;
	int i, j, noofcommands = 2;

	gettimeofday(&start, NULL);
	for (int counter = 0; counter < noofcommands; counter++)
	{
		fscanf(fp, "%c %d %d\n", &rcidentifier, &i, &j);
		shufflerc(matrix, r, c, rcidentifier, i, j,num_thread);
	}

	fclose(fp);
	FILE *outFPtr = fopen(argv[5], "w");
	writeToFile(outFPtr, matrix, r, c);

	// end of execution
	gettimeofday(&end, NULL);

	// free matrix to prevent memory leaks
	for (int i = 0; i < r; i++)
		free(matrix[i]);
	free(matrix);

	// execution time calculation
	float exec_time = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	exec_time /= 1000000;
	printf("Time taken to execute: %fs\n\n", exec_time);

	return 0;
}

// functions
void writeToFile(FILE *outFPtr, int **matrix, int r, int c)
{

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			fprintf(outFPtr, "%d ", matrix[i][j]);

		fprintf(outFPtr, "\n");
	}
}

// function(2) - join the arrays then shuffle it put it back
void shufflerc(int **matrix, int r, int c, char rcidentifier, int i, int j,int num_thread)
{
	

		if (rcidentifier == 'R')
		{
			printf("thread = %d\n", num_thread);
			omp_set_dynamic(0);     // Explicitly disable dynamic teams
			omp_set_num_threads(num_thread); // Use 4 threads for all consecutive parallel regions
			#pragma omp parallel
			{
				int nthreads = omp_get_num_threads();
				printf("Number of threads = %d\n", nthreads);
				int merge[2 * c];

				//both merge operations are independent and there is no need for a barrier!!
				//we do not need an implicit barrier after this loop so we can use no wait
				#pragma omp for nowait
				for (int a = 0; a < c; a++)//merge op1
				{
					// i th row
					merge[a] = matrix[i][a];
				}

				#pragma omp for
				for (int a = 0; a < c; a++)//mergeop2
				{
					// j th row
					merge[a + c] = matrix[j][a];
					//memory operations
				}
				//implicit barrier

				srand(time(NULL));
				#pragma omp for
				for (int s = 0; s < 2 * c; s++)
				{
					int swap_index = rand() % 2 * c;
					int temp = merge[s];
					merge[s] = merge[swap_index];
					merge[swap_index] = temp;
				}
				//implicit barrier 

				//both split operations are independent and there is no need for a barrier!!
				//we do not need an implicit barrier after this loop so we can use no wait
				#pragma omp for nowait
				for (int a = 0; a < c; a++)
				{
					// i th row
					matrix[i][a] = merge[a];
				}
				#pragma omp for
				for (int a = 0; a < c; a++)
				{
					// j th row
					matrix[j][a] = merge[a + c];
				}
				//implicit barrier
			}
		}


	else if (rcidentifier == 'C')
	{ 
		#pragma omp parallel
		{
			int merge[2 * r];
			#pragma omp for nowait
			for (int a = 0; a < r; a++)
			{
				// i th row
				merge[a] = matrix[a][i];
			}

			#pragma omp for
			for (int a = 0; a < r; a++)
			{
				// j th row
				merge[a + r] = matrix[a][j];
			}

			#pragma omp for
			for (int s = 0; s < 2 * r; s++)
			{
				int swap_index = rand() % 2 * r;
				int temp = merge[s];
				merge[s] = merge[swap_index];
				merge[swap_index] = temp;
			}
			// shuffle(2)
			srand(time(NULL));
			#pragma omp for
			for (int s = 0; s < 2 * r; s++)
			{
				int swap_index = rand() % 2 * r;
				int temp = merge[s];
				merge[s] = merge[swap_index];
				merge[swap_index] = temp;
			}
			// split the merged array shuffle achieved
			#pragma omp for nowait
			for (int a = 0; a < r; a++)
			{
				// i th row
				matrix[i][a] = merge[a];
			}
			#pragma omp for
			for (int a = 0; a < r; a++)
			{
				// j th row
				matrix[j][a] = merge[a + r];
			}
		}
	}
}
/*
random comments
rccommands file will look like this
	R 2 4   max will be R 9999 9999?
	C 5 6
	R 9 10
	R 120 34
parllelize file handling? -- please never do this 
overall cannot write a parallel program for this because memory acesss is VERY Costly
you will not get speed up even if you cry for a week :*
*/
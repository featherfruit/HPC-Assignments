/*
	Author:Featherfruit on Github
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>
#include<omp.h>

int main(int argc, char *argv[])
{
	if( argc != 7 ) 
	{
		printf("Invaild  number of Arguments entered \n");
		return -1;
	}

	int r, c,num_thread;
	float p;
	r = atoi(argv[1]);
	c = atoi(argv[2]);
	p = atof(argv[4]);
	num_thread = atoi(argv[6]);

	srand( time(NULL) );
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int **matrix = (int**)malloc(r*sizeof(int*));
		for(int i = 0; i < r; i++)
			matrix[i] = malloc(sizeof(int)*c);
	
	int **binaryimage = (int**)malloc(r*sizeof(int*));
		for(int i = 0; i < r; i++)
			binaryimage[i] = malloc(sizeof(int)*c);

	void writeToFile(FILE*, int**, int, int);
	int pixelPercentageCompare(int **,int , int ,float, int , int,int);

	FILE *fp = fopen(argv[3],"r");
	if (fp == NULL)
		return 0;
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++){

			fscanf(fp, "%d", &matrix[i][j]);
		}
	}
	fclose(fp);

	int i,j;
	omp_set_dynamic(0);
	omp_set_num_threads(num_thread);
	#pragma omp parallel for collapse(2) shared(binaryimage) private(i,j)
	for(i = 0; i < r; i++)
	{
		for(j = 0; j < c; j++)
		{
			int b = pixelPercentageCompare(matrix,r,c, p,i,j,num_thread);
			if(b == 1)
			{
				binaryimage[i][j] = 1;
			}
			else if (b == 0)
			{
				binaryimage[i][j] = 0;
			}
		}
	}
	
	FILE *outFPtr = fopen(argv[5], "w");
	writeToFile(outFPtr,binaryimage, r, c);

	for (int i = 0; i < r; i++)
	free(matrix[i]);
	free(matrix);

	for (int i = 0; i < r; i++)
	free(binaryimage[i]);
	free(binaryimage);

	//end of execution
	gettimeofday(&end, NULL);

	//execution time calculation
	float exec_time = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	exec_time /= 1000000;
	printf("Time taken to execute: %fs\n\n", exec_time);

}	
	
int pixelPercentageCompare(int **matrix,int r, int c, float p, int i, int j,int num_thread)
{
	int count=0;
		
		for(int a = 0; a < r; a++)
		{
			for(int b = 0; b < c; b++)
			{
				if(matrix[i][j] > matrix[a][b])
				{
					count++;
				}
				
			}
		}
	//memory acesss = no point in trying to parallelise it costly operation
			
		
	float k = ((count*100)/(r*c));
	//printf("k: %lf i: %d  j: %d\n",k,i,j);
	if(k < p) return 1;
	else return 0;
}


//function 2 write to file
void writeToFile(FILE *outFPtr, int **matrix, int r, int c)
{
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
			fprintf(outFPtr, "%d ", matrix[i][j]);

		fprintf(outFPtr, "\n");
	}
}

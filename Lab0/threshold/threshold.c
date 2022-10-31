/*
	Author:Featherfruit on Github
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>

//please ignore commented printf statments, they were used for debugging
//need to fix execution time for more accuracy

//command line arguments start
int main(int argc, char *argv[])
{
	if( argc != 6 ) 
	{
		printf("Invaild  number of Arguments entered \n");
		return -1;
	}

	int r, c;
	float p;
	//convert string to integer values
	r = atoi(argv[1]);
	c = atoi(argv[2]);
	p = atof(argv[4]);

	srand( time(NULL) );
	//to calculate executiontime
	struct timeval start, end;
	//start of execution
	gettimeofday(&start, NULL);


	//memory allocation for Matrix M
	int **matrix = (int**)malloc(r*sizeof(int*));
		for(int i = 0; i < r; i++)
			matrix[i] = malloc(sizeof(int)*c);
	

	//Memory allocation for Binary image
	int **binaryimage = (int**)malloc(r*sizeof(int*));
		for(int i = 0; i < r; i++)
			binaryimage[i] = malloc(sizeof(int)*c);

	//function prototypes
	void writeToFile(FILE*, int**, int, int);
	int pixelPercentageCompare(int **,int , int ,float, int , int);


	//file handling starts

	//File containing M (1)
	FILE *fp = fopen(argv[3],"r");
	// Handling edge cases
	if (fp == NULL)
		return 0;
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++){

			fscanf(fp, "%d", &matrix[i][j]);
		}
	}
	fclose(fp);

	//checking if we are able to read M as it is
	// for(int i = 0; i < r; i++)
	// {
	// 	for(int j = 0; j < c; j++)
	// 	{
	// 		printf("%d ",matrix[i][j]);
	// 	}
	// 	printf("\n");
	// }

	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
		{
			//printf("%d i = %d j = %d\n",matrix[i][j],i,j); // works for all cases
			//store the function return value (bij=1 or 0) in int b
			int b = pixelPercentageCompare(matrix,r,c, p,i,j); // doesnt work for i = 0 ,1
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
	//write binary image to output file
	FILE *outFPtr = fopen(argv[5], "w");
	writeToFile(outFPtr,binaryimage, r, c);


	//free matrix to prevent memory leaks?
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
	


//functions

//function (1) set Bij = 1 if p percent of pixels in matrix M are greater than Mij.
int pixelPercentageCompare(int **matrix,int r, int c, float p, int i, int j)
{
	//printf("statment 1 %d i = %d j = %d\n",matrix[i][j],i,j); // works for all cases
	int count=0;
		for(int a = 0; a < r; a++)
		{
			for(int b = 0; b < c; b++)
			{
				//printf("statment 2 i = %d j = %d matrix[i][j] = %d matrix[a][b] = %d  a = %d b = %d count = %d\n",i,j,matrix[i][j],matrix[a][b],a,b,count);
				if(matrix[i][j] > matrix[a][b])
				{
					count++;
				
				}
				
			}
		}
		
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

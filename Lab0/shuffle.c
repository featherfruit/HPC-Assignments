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
#include<sys/time.h>

//command line arguments start
int main(int argc, char *argv[])
{
	if( argc != 6 ) 
	{
		printf("Invaild  number of Arguments entered \n");
		return -1;
	}

	int r, c;
	//convert string to integer values
	r = atoi(argv[1]);
	c = atoi(argv[2]);

	int **matrix = (int**)malloc(c*sizeof(int*));
		for(int i = 0; i < r; i++)
			matrix[i] = malloc(sizeof(int)*c);

	//function prototypes
	void shufflerc (int **,int,int,char,int,int);
	void writeToFile(FILE*, int**, int, int);


	//file handling starts

	// declaring and opening the file containing input matrix (1)
	FILE *fp = fopen(argv[3],"r");
	
	// Handling edge cases
	if (fp == NULL)
		return 0;

	int a;
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
			fscanf(fp, "%d", &matrix[i][j]);
	}
	fclose(fp);

	fp = fopen(argv[4],"r");

	// Handling edge cases
	if (fp == NULL)
	{
		printf("fp was null case use a file with commands");
		return 0;
	}
	char rcidentifier;
	int i,j,noofcommands =1;
	
	for(int counter = 0; counter < noofcommands; counter++)
	{	
		fscanf(fp,"%c %d %d\n",&rcidentifier,&i,&j);
		shufflerc(matrix,r,c,rcidentifier,i,j);

	}
	fclose(fp);
	//writing shuffled matrix to outputmatrix file handling(3)
	FILE *outFPtr = fopen(argv[5], "w");
	writeToFile(outFPtr,matrix, r, c);

	return 0;  

}

//functions
void writeToFile(FILE *outFPtr, int **matrix, int r, int c)
{
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
			fprintf(outFPtr, "%d ", matrix[i][j]);

		fprintf(outFPtr, "\n");
	}
}

//function 2 - join the arrays then shuffle it put it back

void shufflerc(int **matrix,int r, int c, char rcidentifier,int i, int j)
{

	if(rcidentifier =='R')
	{	//merge(1)
		int merge[2*c];
		for(int a=0;a<c;a++)
		{
			//i th row
			merge[a]=matrix[i][a];
		}

		for(int a=0;a<c;a++)
		{
			//j th row
			merge[a+c]=matrix[j][a];
		}
		//shuffle(2)
		// seed the random number generator with the current time to ensure random 
		//numbers generated are different each time our program runs
		srand(time(NULL));

		for (int s = 0; s < 2*c; s++)
		{
			int swap_index = rand() % 2*c;
			int temp = merge[s];
			merge[s] = merge[swap_index];
			merge[swap_index] = temp;
		}
		//split the merged array shuffle achieved(3)
		for(int a=0;a<c;a++)
		{
			//i th row
			matrix[i][a]=merge[a];
		}

		for(int a=0;a<c;a++)
		{
			//j th row
			matrix[j][a]=merge[a+c];
		}
	}
	else if(rcidentifier == 'C')
	{	//merge(1)
		int merge[2*r];
		for(int a=0;a<r;a++)
		{
			//i th row
			merge[a]=matrix[a][i];
		}

		for(int a=0;a<r;a++)
		{
			//j th row
			merge[a+r]=matrix[a][j];
		}
		for (int s = 0; s < 2*r; s++)
		{
			int swap_index = rand() % 2*r;
			int temp = merge[s];
			merge[s] = merge[swap_index];
			merge[swap_index] = temp;
		}
		//shuffle(2)
		srand(time(NULL));
		for (int s = 0; s < 2*r; s++)
		{
			int swap_index = rand() % 2*r;
			int temp = merge[s];
			merge[s] = merge[swap_index];
			merge[swap_index] = temp;
		}
		//split the merged array shuffle achieved
		for(int a=0;a<r;a++)
		{
			//i th row
			matrix[i][a]=merge[a];
		}

		for(int a=0;a<r;a++)
		{
			//j th row
			matrix[j][a]=merge[a+r];
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
parllelize file handling?
*/

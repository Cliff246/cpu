#include <stdio.h>

int matrix1[9] = {
	1, 2, 3,
	4, 5, 6,
	7, 8, 9
};

int matrix2[9] = {
	-3, -4, -5,
	-10, -8, 6,
	12, -12, 20
};

int matrix3[9] = {0};


void multiply_matrix(int *a, int *b, int *c, int size)
{

	for(int x = 0; x < size; ++x)
	{
		int rX = x * size;
		for(int y = 0; y < size; ++y)
		{
			for(int k = 0; k < size; ++k)
			{
				c[rX + y] += a[rX + k] * b[(k * size) + y];
			}
		}
	}
}



int main(int argc, char *argv[])
{

	multiply_matrix(matrix1, matrix2, matrix3, 3);


	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			printf("%d\n", matrix3[(i * 3) + j]);
		}
	}
}
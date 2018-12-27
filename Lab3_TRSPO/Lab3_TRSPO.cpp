#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <time.h>

using namespace std;

void printMatrix(double** matrix, int size);
void makeDiagonalMatrix(double** matrix, int size);
void showAnswer(double** matrix, int size);
void copyMatrix(double** sourceM, double** destM, int size);
void makeAnswer(double** matrix, int size);
bool checkAnswer(double** res, double** matrix, int size);

int main()
{
	setlocale(LC_ALL, "ru");
	double** matrix, **res;
	int size = 0;
	cout << "Введите кол-во переменных: ";
	cin >> size;
	cout << endl;
	matrix = new double*[size];
	res = new double*[size];
	for (int i = 0;i < size;i++)
	{
		matrix[i] = new double[size+1];
		res[i] = new double[size + 1];
	}
	for (int i = 0;i < size;i++)
	{
		printf("Введите коефициенты уравнения %d от а%d1 до а%d%d и свободный член\n", i + 1, i + 1, i + 1, size);
		for (int j = 0;j < size+1;j++)
		{
			cin >> matrix[i][j];
		}
	}
	clock_t start = clock();
	copyMatrix(matrix, res, size);
	printMatrix(res, size);
	makeDiagonalMatrix(res, size);
	printMatrix(res, size);
	makeAnswer(res, size);
	showAnswer(res, size);
	if (checkAnswer(res, matrix, size))
		cout << "Ok\n";
	else
		cout << "Not Ok\n";
	start = clock() - start;
	cout << "Тактов процессора: " << start<<endl;
	system("pause");
    return 0;
}

void printMatrix(double** matrix, int size) 
{
	for (int i = 0;i < size;i++)
	{
		for (int j = 0;j < size;j++)
		{
			cout << setw(6) << matrix[i][j];
		}
		cout << "|" << setw(6) << matrix[i][size] << endl;
	}
	cout << endl;
}

void makeDiagonalMatrix(double** matrix, int size)
{
	for (int iteration = 0;iteration < size-1;iteration++)
	{
		for (int rows = iteration+1;rows < size;rows++)
		{
			double koef = matrix[rows][iteration]/matrix[iteration][iteration];
			omp_set_num_threads(4);
			#pragma omp parallel for 
			for (int columns = iteration;columns < size + 1;columns++)
			{
				matrix[rows][columns] -= matrix[iteration][columns] * koef;
			}
		}
	}
	for (int iteration = size-1;iteration >=0;iteration--)
	{
		for (int rows = iteration - 1;rows >=0;rows--)
		{
			double koef = matrix[rows][iteration] / matrix[iteration][iteration];
			omp_set_num_threads(4);
			#pragma omp parallel for
			for (int columns = size;columns >=rows;columns--)
			{
				matrix[rows][columns] -= matrix[iteration][columns] * koef;
			}
		}
	}
	

}

void showAnswer(double** matrix, int size)
{
	for (int i = 0;i < size;i++)
	{
		printf("Х%d=%.2f\n",i,matrix[i][i]);
	}
	cout << endl;
}

void makeAnswer(double** matrix, int size)
{
	omp_set_num_threads(4);
	#pragma omp parallel for
	for (int i = 0;i < size;i++)
	{
		matrix[i][i] = matrix[i][size] / matrix[i][i];
	}
}

void copyMatrix(double** sourceM, double** destM,int size)
{
	omp_set_num_threads(4);
	#pragma omp parallel for
	for (int i = 0;i < size;i++)
	{
		for (int j = 0;j < size + 1;j++)
		{
			destM[i][j] = sourceM[i][j];
		}
	}
}

bool checkAnswer(double** res, double** matrix, int size)
{
	for (int i = 0;i < size;i++)
	{
		double sum = 0;
		omp_set_num_threads(4);
		#pragma omp parallel for shared(sum)
		for (int j = 0;j < size;j++)
		{
			sum += matrix[i][j] * res[j][j];
		}
		if (sum != matrix[i][size])
			return false;
	}
	return true;
}


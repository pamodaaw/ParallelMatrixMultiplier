#include <iostream>
#include <stdlib.h>
#include <random>
#include <ctime>
#include <chrono>
#include <omp.h>
using namespace std;
using namespace std::chrono;


double** createMatrix(int n){
	int i;
	double ** matrix;

	matrix = (double **) malloc(sizeof(double *) * n);

	for (i = 0; i < n; i++) {
	    matrix[i] = (double *) malloc(sizeof(double) * n);
	}

	return matrix;
}

void initializeMatrix(double** matrix, int n){

	//fill the matrix with random double values
	//time based seed for random generator
  	unsigned seed = system_clock::now().time_since_epoch().count();
 	default_random_engine gen (seed);
    uniform_real_distribution<> dis(1, 10000);
	// Fill the matrix with the random numbers
	for (int row = 0; row < n; row++) {
		for (int col = 0; col < n; col++) {
			double value =dis(gen);
			matrix[row][col] = value;
		}
	}
}

double** generateRandMatrix(int size){
	double** matrix = createMatrix(size);
	initializeMatrix(matrix, size);
	return matrix;
}
void transpose(double** A, double** B, int n) {
    int i,j;
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			B[i][j] = A[j][i];
		}
	}
}
void print_matrix(char * name, double** matrix, int size){
	int i, j;

	printf("%s\n", name);

	for (i=0; i < size; i++){
		for (j=0; j < size; j++){
			printf("%.3f\t", matrix[i][j]);
		}
		printf("\n");
	}
}
int min(int a, int b){
	if(a<b){
		return a;
	}
	else
		return b;
}

long getElapsedTimeForMultiplying(double** matrixA, double** matrixB, int size){
	double** resultant = createMatrix(size);

	// Starting clock
	high_resolution_clock::time_point start = high_resolution_clock::now();
    
    //define blocks 
    int blockSize = 20;
   	int no_of_blocks;
    if (size<20)
       blockSize = 1;
   	else
   		no_of_blocks = size/blockSize;

   	//parallel loop for the matrix multiplication
	double sum;
    int i0,j0,k0,i,j,k;
	#pragma omp parallel for shared(matrixA,  matrixB, resultant) private(i,j,k, sum) schedule(static) num_threads(blockSize)
	for (i0 = 0; i0 < size; i0 = i0 + no_of_blocks) {
		for (j0 = 0; j0 < size; j0 = j0 + no_of_blocks) {
			for (k0 = 0; k0 < size; k0 = k0 + no_of_blocks) {
				for (i=i0; i < min(i0+no_of_blocks,size); i++){
					for (j=j0; j < min(j0+no_of_blocks,size); j++){
						sum = 0;
						for(k=k0; k < min(k0+no_of_blocks,size); k++){
							sum += matrixA[i][k] * matrixB[k][j];

						}
						resultant[i][j] += sum;

					}
				}
			}	
		}	
	}
	// Ending clock
	high_resolution_clock::time_point end = high_resolution_clock::now();
	long duration = duration_cast<milliseconds>( end - start ).count();

	//clears memory
	free(matrixA);
	free(matrixB);
	free(resultant);

	// Returns the elapsed time
	return duration;
}

void multiplyTwoRandMatrices(int size, int sampleSize){
	double timeTotal = 0;
	long elapsedTime = 0;
    double timesTaken[sampleSize];
	double squareSum = 0;
	double timeAverage;
	double standardDeviation;
	int i;

	for(i=0; i <sampleSize; i++){
		double** matrixA = generateRandMatrix(size);
		double** matrixB = generateRandMatrix(size);
		elapsedTime = getElapsedTimeForMultiplying(matrixA, matrixB, size);
		timeTotal += elapsedTime ;
		timesTaken[i] = elapsedTime;
	}
	
	//calculate the mean
	timeAverage = (double)(timeTotal/(sampleSize-1) );

	//calculate standard deviation
	for(i=0; i <sampleSize; i++){
		squareSum += (timesTaken[i] - timeAverage) * (timesTaken[i] - timeAverage);
	}
	standardDeviation = sqrt(squareSum /sampleSize );
	printf("Average Time Spent for sample size of %d  : %.2f milli secs\n", sampleSize, timeAverage);
	printf("standard deviation  : %.2f milli secs\n", standardDeviation);
}


int main(int argc, const char* argv[]) {
	if (argc == 2){
		int size = atoi(argv[1]);
		int sampleSize = 100;
		multiplyTwoRandMatrices(size,sampleSize);
	}
	else
		cout << "Invalid input value";

}



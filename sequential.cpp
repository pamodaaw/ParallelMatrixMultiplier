#include <iostream>
#include <stdlib.h>
#include <random>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;


double** createMatrix(int n){
	double** matrix = new double*[n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new double[n];
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

long getElapsedTimeForMultiplying(double** matrixA, double** matrixB, int size){
	double** resultant = createMatrix(size);
	// Starting clock
	high_resolution_clock::time_point start = high_resolution_clock::now();

	//multiplication
	for(int row = 0; row < size; row++){
	    for(int col = 0; col < size; col++){
			resultant[row][col] = 0.0;
	        for (int cur = 0; cur < size; cur++) {
	            resultant[row][col] += matrixA[row][cur] * matrixB[cur][col];
	        }
	    }
	}
	// Ending clock
	high_resolution_clock::time_point end = high_resolution_clock::now();
	long duration = duration_cast<milliseconds>( end - start ).count();
	// Clears the memory
	delete matrixA;
	delete matrixB;
	delete resultant;

	// Returns the elapsed time
	return duration;
}

void multiplyTwoRandMatrices(int size){
	double** matrixA = generateRandMatrix(size);
	double** matrixB = generateRandMatrix(size);
	cout << getElapsedTimeForMultiplying(matrixA, matrixB, size);
}



int main(int argc, const char* argv[]) {
	if (argc == 2){
		int size = atoi(argv[1]);
		multiplyTwoRandMatrices(size);
	}
	else
		cout << "Invalid input value";

}




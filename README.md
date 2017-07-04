# ParallelMatrixMultiplier

### Sequential program:
* compile : g++ -std=c++11 sequential.cpp -o sequential
* run : ./sequential <matrix_size>

### Parallel program:
* compile: g++ -std=c++11 parallel.cpp -o parallel -fopenmp
* run : ./parallel <matrix_size>

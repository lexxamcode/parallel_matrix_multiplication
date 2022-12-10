#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>

int** load_matrix(const char* filename, size_t size)
{
    FILE* file;

    int** matrix = (int**)malloc(sizeof(int*)*size);

    for (size_t i = 0; i < size; i++)
    {
        matrix[i] = (int*)malloc(sizeof(int)*size);
    }

    if ((file = fopen(filename, "r")) == NULL)
        printf("Cannot open file.\n");

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
            fscanf(file, "%d ", &matrix[i][j]);
    }

    return matrix;
}

void write_matrix_to_file(int** matrix, size_t size, const char* name)
{
    FILE* file;

    if ((file = fopen(name, "w")) == NULL)
        printf("Cannot open file.\n");

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
            fprintf(file, "%d ", matrix[i][j]);
        fprintf(file, "\n");
    }
}

int** multiply_matrices(int** a, int** b, size_t size)
{
    int** c = (int**)malloc(sizeof(int*)*size);
    for (size_t i = 0; i < size; i++)
    {
        c[i] = (int*)malloc(sizeof(int)*size);
    }

    size_t i, j, k;
    unsigned start_time = clock();
    int threadnum = 4;

    #pragma omp parallel for shared(matrix_a, matrix_b, result) private(i, j, k) num_threads(threadnum)
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            c[i][j] = 0;
            for (k = 0; k < size; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    unsigned end_time = clock() - start_time;
    printf("For %d threads and %d size: %d seconds;\n", threadnum, size, end_time);
    return c;
}

int main(int argc, char* argv[])
{
    size_t size;
    if (argc == 1)
        size = 10;
    if (argc == 2)
        sscanf(argv[1], "%u", &size);

    int** a_matrix = load_matrix("a.txt", size);
    int** b_matrix = load_matrix("b.txt", size);
    int** c_matrix = multiply_matrices(a_matrix, b_matrix, size);
    write_matrix_to_file(c_matrix, size, "c.txt");

    return 0;
}
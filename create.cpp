#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int** create_matrix(size_t size)
{
    srand(time(NULL) + rand()%10);
    int** a = (int**)malloc(sizeof(int*)*size);

    for (size_t i = 0; i < size; i++)
        a[i] = (int*)malloc(sizeof(int)*size);

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            a[i][j] = rand()%10;
        }
    }

    return a;
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

int main(int argc, char* argv[])
{
    size_t size;
    if (argc == 1)
        size = 10;
    if (argc == 2)
        sscanf(argv[1], "%u", &size);

    int** a_matrix = create_matrix(size);
    write_matrix_to_file(a_matrix, size, "a.txt");

    int** b_matrix = create_matrix(size);
    write_matrix_to_file(b_matrix, size, "b.txt");

    for (size_t i = 0; i < size; i++)
    {
        free(a_matrix[i]);
        free(b_matrix[i]);
    }
    free(a_matrix);
    free(b_matrix);

    return 0;
}
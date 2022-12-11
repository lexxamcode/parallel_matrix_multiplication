#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"

int* load_matrix(const char* filename, size_t size)
{
    FILE* file;

    int* matrix = (int*)malloc(sizeof(int)*size*size);

    if ((file = fopen(filename, "r")) == NULL)
        printf("Cannot open file.\n");

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
            fscanf(file, "%d ", &matrix[i*size + j]);
    }

    return matrix;
}

void write_matrix_to_file(int* matrix, size_t size, const char* name)
{
    FILE* file;

    if ((file = fopen(name, "w")) == NULL)
        printf("Cannot open file.\n");

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
            fprintf(file, "%d ", matrix[i*size + j]);
        fprintf(file, "\n");
    }
}

// MPI routines
int ProcNum, ProcRank;

//————————————————-
void Flip (int *B, size_t size)
{
	int temp=0;
	for (size_t i=0;i<size;i++)
	{
		for (size_t j=i+1;j<size;j++)
		{
			temp = B[i*size+j];
			B[i*size+j] = B[j*size+i];
			B[j*size+i] = temp;
		}
	}
}

void MPI_mmult(int *A, int *B, int *C, size_t size)
{
	size_t dim = size;
	int i, j, k, p, ind;
	int temp;
	MPI_Status Status;
	int ProcPartSize = dim/ProcNum;
	int ProcPartElem = ProcPartSize*dim;
	int* bufA = (int*)malloc(sizeof(int)*ProcPartElem);
	int* bufB = (int*)malloc(sizeof(int)*ProcPartElem);
	int* bufC = (int*)malloc(sizeof(int)*ProcPartElem);
	int ProcPart = dim/ProcNum, part = ProcPart*dim;

	if (ProcRank == 0)
		Flip(B, size);

	MPI_Scatter(A, part, MPI_INT, bufA, part, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, part, MPI_INT, bufB, part, MPI_INT, 0, MPI_COMM_WORLD);

	temp = 0;
	for (i=0; i < ProcPartSize; i++) {
		for (j=0; j < ProcPartSize; j++) {
			for (k=0; k < dim; k++)
				temp += bufA[i*dim+k]*bufB[j*dim+k];
			bufC[i*dim+j+ProcPartSize*ProcRank] = temp;
			temp = 0;
		}
	}

	int NextProc; int PrevProc;
	for (p=1; p < ProcNum; p++) {
		NextProc = ProcRank+1;
		if (ProcRank == ProcNum-1)
			NextProc = 0;
		PrevProc = ProcRank-1;
		if (ProcRank == 0)
			PrevProc = ProcNum-1;
		MPI_Sendrecv_replace(bufB, part, MPI_INT, NextProc, 0, PrevProc, 0, MPI_COMM_WORLD, &Status);
		temp = 0;
		for (i=0; i < ProcPartSize; i++) {
			for (j=0; j < ProcPartSize; j++) {
				for (k=0; k < dim; k++) {
					temp += bufA[i*dim+k]*bufB[j*dim+k];
				}
				if (ProcRank-p >= 0 )
					ind = ProcRank-p;
				else ind = (ProcNum-p+ProcRank);
				bufC[i*dim+j+ind*ProcPartSize] = temp;
				temp = 0;
			}
		}
	}

	MPI_Gather(bufC, ProcPartElem, MPI_INT, C, ProcPartElem, MPI_INT, 0, MPI_COMM_WORLD);

	free(bufA);
	free(bufB);
	free(bufC);

	MPI_Finalize();
}

int main(int argc, char *argv[])
{
    clock_t start;
    if (argc != 2)
    {
	printf("Program usage: \n./%s <n>\nwhere <n> is the size of square matrix\n", argv[0]);
	return -1;
    }

    const size_t size = atoi(argv[1]);

    if (ProcRank == 0)
	printf("Begin initializing ...\n");

    // Load matrices from fiels:
    int* a_matrix = load_matrix("a.txt", size);
    int* b_matrix = load_matrix("b.txt", size);

    int* c_matrix = (int*)malloc(sizeof(int)*size*size);
    if (ProcRank == 0)
    	printf("Begin calculating ...\n");

	start = clock();

	// Begin MPI
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	MPI_mmult(a_matrix, b_matrix, c_matrix, size);

    /* Write result to file if main thread*/
    if (ProcRank == 0)
    {
    	write_matrix_to_file(c_matrix, size, "c.txt");
    	printf("(%d Threads; %d Size): %f seconds.\n", ProcNum, size, double(clock() - start)/CLOCKS_PER_SEC);
    }

    // free memory
    free(a_matrix);
    free(b_matrix);
    free(c_matrix);

    return 0;
}

//
// Created by Alexey on 17.09.2022.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <omp.h>
#include <ctime>
#include <fcntl.h>
#include <string>

namespace lab1
{
    typedef std::vector<std::vector<float>> fmatrix;

//Makes random matrix needed size
    fmatrix makeMatrix(const size_t &rows, const size_t &columns)
    {
        fmatrix matrix(rows);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(1, 10);
        size_t i, j;

        #pragma omp parallel for shared(matrix) private(i, j)
        for (i = 0; i < rows; i++)
        {
            matrix[i].resize(columns);

            for (j = 0; j < columns; j++)
                matrix[i][j] = dist(gen);
        }

        return matrix;
    }

    void printMatrix(const fmatrix& matrix)
    {
        for (size_t i = 0; i < matrix.size(); i++)
        {
            for (size_t j = 0; j < matrix[i].size(); j++)
                std::cout << matrix[i][j] << " ";
            std::cout << std::endl;
        }
    }

    void serializeMatrix(const fmatrix& matrix, const std::string& filename)
    {
        std::ofstream mout(filename, std::ios::out);
        if (!mout) throw "Can not open the file";

        for (size_t i = 0; i < matrix.size(); i++)
        {
            for (size_t j = 0; j < matrix[i].size() - 1; j++)
            {
                mout << matrix[i][j] << " ";
            }
            mout << matrix[i][matrix[i].size() - 1];
            mout << std::endl;
        }

        mout.close();
    }

    fmatrix loadMatrix(const std::string& filename)
    {
        std::ifstream min(filename, std::ios::in);
        if (!min) throw "Can not open the file";

        fmatrix matrix;
        std::vector<float> temp;
        float temp_element;

        while(min >> temp_element)
        {
            temp.push_back(temp_element);
            if (min.peek() == '\n' || min.peek() == EOF)
            {
                matrix.push_back(temp);
                temp.clear();
            }
        }
        min.close();
        return matrix;
    }

//serial matrix multiply
    std::pair<fmatrix, unsigned> multiplyMatrices(const fmatrix& matrix_a, const fmatrix& matrix_b)
    {
        if (matrix_a[0].size() != matrix_b.size())
            throw "Matrices can not be multiplied";

        fmatrix result(matrix_a.size());
        unsigned start_time = clock();
        for (size_t i = 0; i < result.size(); i++)
        {
            result[i].resize(matrix_b[0].size());
            for (size_t j = 0; j < matrix_b[0].size(); j++)
            {
                result[i][j] = 0;
                for (size_t k = 0; k < matrix_b.size(); k++)
                {
                    result[i][j] += matrix_a[i][k]*matrix_b[k][j];
                }
            }
        }
        unsigned end_time = clock();
        std::pair<fmatrix, unsigned> result_pair = {result, end_time-start_time};

        return result_pair;
    }

//coroutine matrix multiply
    std::pair<fmatrix,unsigned> pMultiplyMatrices(const fmatrix& matrix_a, const fmatrix& matrix_b)
    {
        if (matrix_a[0].size() != matrix_b.size())
            throw "Matrices can not be multiplied";

        fmatrix result(matrix_a.size());
        size_t i, j, k;
        unsigned start_time = clock();

        #pragma omp parallel for shared(matrix_a, matrix_b, result) private(i, j, k)
        for (i = 0; i < result.size(); i++)
        {
            result[i].resize(matrix_b[0].size());
            for (j = 0; j < matrix_b[0].size(); j++)
            {
                result[i][j] = 0;
                for (k = 0; k < matrix_b.size(); k++)
                {
                    result[i][j] += matrix_a[i][k]*matrix_b[k][j];
                }
            }
        }
        unsigned end_time = clock();
        std::pair<fmatrix, unsigned> result_pair = {result, end_time - start_time};
        return result_pair;
    }
}



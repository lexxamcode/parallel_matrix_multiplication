//
// Created by lexam on 14.10.2022.
//

#include "functions.h"
#include <iostream>

using namespace lab1;

int main()
{
    for (int i = 50; i <= 1600; i *=2)
    {
        fmatrix matrix_a = makeMatrix(i, i);
        fmatrix matrix_b = makeMatrix(i, i);
        fmatrix matrix_c = pMultiplyMatrices(matrix_a, matrix_b).first;
        serializeMatrix(matrix_c, "../result.txt");
        serializeMatrix(matrix_a, "../a.txt");
        serializeMatrix(matrix_b, "../b.txt");
        system("python ../multiplication_checker.py");
    }
    return 0;
}

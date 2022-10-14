import numpy
import numpy as np


def load_matrix(filename: str):
    matrix = []
    with open(filename, 'r') as matrix_file:
        for line in matrix_file:
            matrix.append([int(x) for x in line.split()])

    return matrix


def matrix_comparison(m1: np.ndarray, m2: np.ndarray):
    if (m1.shape[0] != m2.shape[0]) or (m1.shape[1] != m2.shape[1]):
        return False

    for i in range(m1.shape[0]):
        for j in range(m1.shape[1]):
            if not m1[i][j] == m2[i][j]:
                return False

    return True


if __name__ == "__main__":
    # c = 50
    # while c <= 1600:
    #     matrix_a = load_matrix(f'entire output/m{c}/a.txt')
    #     matrix_b = load_matrix(f'entire output/m{c}/b.txt')
    #     matrix_c = load_matrix(f'entire output/m{c}/c.txt')
    #     matrix_c_check = np.dot(np.asarray(matrix_a), np.asarray(matrix_b))
    #
    #     print(matrix_c[0][0])
    #     print(matrix_c_check[0][0])
    #     print("\n")
    #
    #     if not matrix_comparison(matrix_c_check, np.asarray(matrix_c)):
    #         print("FALSE")
    #     else:
    #         print("TRUE")
    #     c *= 2
    matrix_a = load_matrix(f'../a.txt')
    matrix_b = load_matrix(f'../b.txt')
    matrix_c = load_matrix(f'../result.txt')
    matrix_c_check = np.dot(matrix_a, matrix_b)
    print(matrix_c[0][0])
    print(matrix_c_check[0][0])
    print(matrix_comparison(matrix_c_check, np.asarray(matrix_c)))

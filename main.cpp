#include "functions.h"

using namespace lab1;

int main()
{
    system("python ../prepare_script.py");

    std::string matrix_a_filename = "a.txt";
    std::string matrix_b_filename = "b.txt";
    std::string matrix_c_filename = "c.txt";
    std::string time_file = "time.txt";

    for (size_t i = 50; i <= 1600; i *= 2)
    {
        std::string entire_path = "../entire output/m";
        entire_path += std::to_string(i) + "/";

        fmatrix a = makeMatrix(i, i);
        fmatrix b = makeMatrix(i, i);
        std::pair<fmatrix, unsigned> result_pair = pMultiplyMatrices(a, b);

        serializeMatrix(a, entire_path+matrix_a_filename);
        serializeMatrix(a, entire_path+matrix_b_filename);
        serializeMatrix(result_pair.first, entire_path+matrix_c_filename);

        std::ofstream tstream(entire_path + time_file);
        tstream << result_pair.second;
        tstream.close();
    }

    system("python ../multiplication_checker.py");

    return 0;
}

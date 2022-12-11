gcc -std=gnu++11 -fopenmp ./create.cpp -o create.o
mpicxx load.cpp -o load.o
for ((size=50; size <=1600; size = size*2)); do
./create.o $size
mpiexec -n 2 ./load.o $size
done

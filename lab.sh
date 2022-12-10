gcc -std=gnu++11 -fopenmp ./create.cpp -o create.o
gcc -std=gnu++11 -fopenmp ./load.cpp -o load.o
for ((size=50; size <=1600; size = size*2)); do
./create.o $size
./load.o $size
done

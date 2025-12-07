all: 
	g++ -O3 -mavx2 merge-sort.cpp merge-serial.hpp merge-avx.hpp -o merge-serial.o
	nvcc merge-cuda.cu merge merge-serial.o -o merge.o

clean: 
	rm merge.o
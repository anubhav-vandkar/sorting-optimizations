all: 
	nvcc  -O3 -Xcompiler -mavx2 merge-sort.cpp merge-cuda.cu  -o merge-sort.o

clean: 
	rm merge-sort.o
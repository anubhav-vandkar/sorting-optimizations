all: 
	nvcc  -O3 -Xcompiler -mavx2 merge-sort.cpp merge-cuda-naive.cu merge-cuda-shared.cu -o merge-sort.o
	./merge-sort.o

clean: 
	rm merge-sort.o
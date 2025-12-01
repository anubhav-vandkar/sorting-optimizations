all: 
	g++ -O3 -mavx2 merge-sort.cpp merge-serial.hpp merge-avx.hpp -o merge

clean: 
	rm merge.O
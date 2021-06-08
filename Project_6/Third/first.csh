#!/bin/csh

#foreach s ( 8 16 32 64 128 256 512 1024 )
foreach n ( 1 2 4 8 16 32 64 128 256 )
	#icpc -o firstx first.cpp -no-vec -DLOCAL_SIZE=$s /scratch/cuda-7.0/lib64/libOpenCL.so -lm -openmp
	icpc -o firstx first.cpp -no-vec -DNMB=$n /scratch/cuda-7.0/lib64/libOpenCL.so -lm -openmp
	./firstx
end

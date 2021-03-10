/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <hip/hip_runtime.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#define SAMPLE_VERSION "HIP-Examples-Application-v1.0"
#define SUCCESS 0
#define FAILURE 1

#define MATRIX_WIDTH 	16
#define MATRIX_HEIGHT	16

#define THREADS_PER_BLOCK_X 	4
#define THREADS_PER_BLOCK_Y 	4

using namespace std;

__global__ void strided_copy(int *in, int *out, int width, int height)
{
	int x = hipThreadIdx_x + hipBlockDim_x * hipBlockIdx_x;
	int y = hipThreadIdx_y + hipBlockDim_y * hipBlockIdx_y;
	//out[y][x] = in[x][y]
	*(out + x*width + y) = *(in + x*width + y);
}

void print_value(int *matrix, int width, int height)
{
	for (int i = 0; i < width; i++)	{
		for (int j = 0; j < height; j++) {
			cout << *(matrix + i * width + j) << "\t";	
		}
		cout << endl;
	}
	return;
}

int main(int argc, char* argv[])
{

    hipDeviceProp_t devProp;
    hipGetDeviceProperties(&devProp, 0);
    cout << " System minor " << devProp.minor << endl;
    cout << " System major " << devProp.major << endl;
    cout << " agent prop name " << devProp.name << endl;

	/* Initial input,output for the host and create memory objects for the kernel*/
	int h_input[MATRIX_WIDTH][MATRIX_HEIGHT];
	int h_output[MATRIX_WIDTH][MATRIX_HEIGHT];

	for (int i = 0; i < MATRIX_WIDTH; i++)	{
		for (int j = 0; j < MATRIX_HEIGHT; j++) {
			h_input[i][j] = i;
		}	
	}

	cout << "input Matrix \n";
	print_value((int *)&h_input, MATRIX_WIDTH, MATRIX_HEIGHT);

	int *d_input;
	int *d_output;
	hipMalloc((void**)&d_input, sizeof(int)*MATRIX_WIDTH*MATRIX_HEIGHT);
    hipMalloc((void**)&d_output, sizeof(int)*MATRIX_WIDTH*MATRIX_HEIGHT);
	
	hipMemcpy(d_input, h_input, sizeof(int)*MATRIX_WIDTH*MATRIX_HEIGHT, hipMemcpyHostToDevice);

	hipLaunchKernelGGL(strided_copy,
                  dim3(MATRIX_WIDTH/THREADS_PER_BLOCK_X, MATRIX_HEIGHT/THREADS_PER_BLOCK_Y),
                  dim3(THREADS_PER_BLOCK_X, THREADS_PER_BLOCK_Y),
                  0, 0,
                  d_input, d_output, MATRIX_WIDTH, MATRIX_HEIGHT);

	hipMemcpy(h_output, d_output, sizeof(int)*MATRIX_WIDTH*MATRIX_HEIGHT, hipMemcpyDeviceToHost);

	cout << "output Matrix \n";
	print_value((int *)&h_output, MATRIX_WIDTH, MATRIX_HEIGHT);

	int errors = 0;
	for (int i = 0; i < MATRIX_WIDTH; i++)	{
		for (int j = 0; j < MATRIX_HEIGHT; j++) {
			if (h_input[i][j] != h_output[i][j]) {
				errors++;
			}
		}	
	}
	
	if (0 == errors) {
		std::cout<<"Passed!\n";
	} else {
		std::cout<<"error occurred!\n";
	}

    hipFree(d_input);
    hipFree(d_output);

	free(h_input);
	free(h_output);

	return SUCCESS;
}

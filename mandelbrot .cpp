//============================================================================
// Name        : ProjectASequential.cpp
// Author      : Ali Abbas
// Version     :
// Copyright   : This is project A of Parallel and Multicore Computing
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio>
#include <stdlib>
#include <omp.h>


// return 1 if in set, 0 otherwise
int inset(double real, double img, int maxiter){
	double z_real = real;
	double z_img = img;
	for(int iters = 0; iters < maxiter; iters++){
		double z2_real = z_real*z_real-z_img*z_img;
		double z2_img = 2.0*z_real*z_img;
		z_real = z2_real + real;
		z_img = z2_img + img;
		if(z_real*z_real + z_img*z_img > 4.0) return 0;
	}
	return 1;
}

// count the number of points in the set, within the region
int mandelbrotSetCount(double real_lower, double real_upper, double img_lower, double img_upper, int num, int maxiter){
	int count=0;
	double real_step = (real_upper-real_lower)/num;
	double img_step = (img_upper-img_lower)/num;
	#pragma omp parallel for reduction (+: count ) schedule(dynamic)
	for(int real=0; real<=num; real++){
		for(int img=0; img<=num; img++){
			count+=inset(real_lower+real*real_step,img_lower+img*img_step,maxiter);
		}
	}
	return count;
}

// main
int main(int argc, char *argv[]){
	double real_lower;
	double real_upper;
	double img_lower;
	double img_upper;
	int num;
	int maxiter;
	int num_regions = (argc-1)/6;

	
	#pragma omp parallel for ordered private(real_lower, real_upper, img_lower, img_upper, num, maxiter)
	for(int region=0;region<num_regions;region++){
		// scan the arguments
		sscanf(argv[region*6+1],"%lf",&real_lower);
		sscanf(argv[region*6+2],"%lf",&real_upper);
		sscanf(argv[region*6+3],"%lf",&img_lower);
		sscanf(argv[region*6+4],"%lf",&img_upper);
		sscanf(argv[region*6+5],"%i",&num);
		sscanf(argv[region*6+6],"%i",&maxiter);

		printf("%d\n",mandelbrotSetCount(real_lower,real_upper,img_lower,img_upper,num,maxiter));
	}
	
	return EXIT_SUCCESS;
}

#ifndef COMPUTEPI_PAR_HPP
#define COMPUTEPI_PAR_HPP


#include <omp.h>




double computepi()
{
	int i , N;
	double h,x,sum,PI;

	N = 100000000;	
	h = 1.0/N;
	sum = 0.0;

	#pragma omp parallel private(i,x) shared(N,h,sum)
	{
		const int id = omp_get_thread_num();
		std::cout<<"Hello from thread"<<id<<std::endl;
		#pragma omp parallel for reduction(+:sum)
   		for (int i = 1; i <= N; i++)
    		{
			x = h* (i-0.5);
			sum+= 4.0 /(1.0+x*x);

		}
	}
	return h*sum;
}

#endif // !_X_GRADIENT_PAR_

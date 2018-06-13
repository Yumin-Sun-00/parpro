#include <iostream>
#include <stdio.h>

#include "computepi_par.hpp"


int main(int argc, char **argv) {

	double pi = computepi();
	std::cout<<pi<<std::endl;
    return 0;
}

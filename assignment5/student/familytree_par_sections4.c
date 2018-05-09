#include "familytree.h"
#include <omp.h>

void traverse(tree *node, int numThreads){

    omp_set_nested(1);
    omp_set_max_active_levels(4);//tested best

	#pragma omp parallel
	{
		if(node != NULL){
			node->IQ = compute_IQ(node->data);
            genius[node->id] = node->IQ;
                
            #pragma omp sections
            {
                #pragma omp section
                {
                    traverse(node->right, numThreads);

                }
                #pragma omp section
                {
                     traverse(node->left, numThreads);
                }
            }
		}
	}
}


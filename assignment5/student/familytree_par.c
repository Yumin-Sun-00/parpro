#include "familytree.h"
#include <omp.h>

//#define T 30

void traverse(tree *node, int numThreads){
	omp_set_num_threads(numThreads);
	
    if(node != NULL){
        node->IQ = compute_IQ(node->data);
        genius[node->id] = node->IQ;
				
        #pragma omp parallel
        {
		    #pragma omp single
            {
                #pragma omp task final(node->id > 10*numThreads)
                {
                    traverse(node->right, numThreads);
                }

                #pragma omp task final(node->id > 10*numThreads)
                {
                    traverse(node->left, numThreads);
                }
            }
        }
	}
}

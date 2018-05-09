#include "familytree.h"
#include <omp.h>

#define T 30

void traverse(tree *node, int numThreads){
	omp_set_num_threads( numThreads);
	if(node != NULL){
                                node->IQ = compute_IQ(node->data);
                                genius[node->id] = node->IQ;
				
	#pragma omp parallel
	{
		#pragma omp single
		{
		//	if(node != NULL){
		//		node->IQ = compute_IQ(node->data);
                //	        genius[node->id] = node->IQ;
                	
	
        			#pragma omp task final(node->id > T)// firstprivate(numThreads) shared(node)//firstprivate(node,numThreads)
        			{
//                              printf("hello1");
                			traverse(node->right, numThreads);
        			}

        			#pragma omp task final(node->id > T)// firstprivate(numThreads) shared(node)//firstprivate(node,numThreads)
       				{
//                              printf("hello2");
                			traverse(node->left, numThreads);
        			}
		//	}
		}
	}
	}
}

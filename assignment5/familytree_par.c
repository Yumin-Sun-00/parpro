#include "familytree.h"
#include <omp.h>

void traverse(tree *node, int numThreads){
	
	#pragma omp parallel firstprivate(numThreads,node)
	{
		#pragma omp single
		{
			if(node != NULL){
				node->IQ = compute_IQ(node->data);
				genius[node->id] = node->IQ;
			}

			#pragma omp task firstprivate(numThreads)
			{		
				printf("hello1");
				traverse(node->right, numThreads);
			}

			#pragma omp task firstprivate(numThreads)
			{
				printf("hello2");
				traverse(node->left, numThreads);
			}
		}
	}

}


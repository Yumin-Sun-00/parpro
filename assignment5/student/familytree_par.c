#include "familytree.h"
#include <omp.h>


//static int curr_depth = 0;

void traverse(tree *node, int numThreads){

omp_set_nested(1);
omp_set_max_active_levels(4);

	#pragma omp parallel
	{
		if(node != NULL){
			node->IQ = compute_IQ(node->data);
                        genius[node->id] = node->IQ;
                
		#pragma omp sections
		{
			#pragma omp section
			{
				//printf("hello");
				// if(node != NULL){
				// 	node->IQ = compute_IQ(node->data);
				// 	genius[node->id] = node->IQ;
				// }
				
				// //curr_depth = curr_depth+1;
				// printf("new section");
				traverse(node->right, numThreads);

			}
			#pragma omp section
			{
				//printf("hello");
				// if(node != NULL){
				// 	node->IQ = compute_IQ(node->data);
				// 	genius[node->id] = node->IQ;
				// }
				// //curr_depth = curr_depth+1;
				// //printf("%d,",curr_depth);
				// printf("new section");
				 traverse(node->left, numThreads);
			}
		}
		}
	}

}


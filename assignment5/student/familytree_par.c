#include "familytree.h"
#include <omp.h>

#define T 20

//void process(tree *node){
//    node->IQ = compute_IQ(node->data);
//    genius[node->id] = node->IQ;
//}

void traverse_kernel(tree *node,int depth){
    if(node!=NULL)
    {
        node->IQ = compute_IQ(node->data);
        genius[node->id] = node->IQ;

        #pragma omp task final(depth > T)
        {traverse_kernel(node->right,depth+1);}

        #pragma omp task final(depth+1 > T)
        {traverse_kernel(node->left,depth+1);}
    }
}

void traverse(tree *node, int numThreads){
    omp_set_num_threads(numThreads);
    #pragma omp parallel
    {
        #pragma omp single
        {
            traverse_kernel(node,0);
        }
    }
}

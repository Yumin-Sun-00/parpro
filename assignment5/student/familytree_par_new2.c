#include "familytree.h"
#include <omp.h>

#define T 30

//void process(tree *node){
//    node->IQ = compute_IQ(node->data);
//    genius[node->id] = node->IQ;
//}

void traverse_kernel(tree *node){
    if(node!=NULL)
    {
        node->IQ = compute_IQ(node->data);
        genius[node->id] = node->IQ;

        #pragma omp task final(node->id > T)
        {traverse_kernel(node->right);}

        #pragma omp task final(node->id > T)
        {traverse(node->left);}
    }
}

void traverse(tree *node, int numThreads){
    #pragma omp parallel
    {
        #pragma omp single
        {
            traverse_kernel(node);
        }
    }
}

#include "familytree.h"
#include <omp.h>

#define T 30

void process(tree *node){
    node->IQ = compute_IQ(node->data);
    genius[node->id] = node->IQ;
}

void traverse(tree *node, int numThreads){
    
    if(node->right){
        #pragma omp task final(node->id > T)
        {
            traverse(node->right, numThreads);
        }
    }
    if(node->left){
        #pragma omp task final(node->id > T)
        {
            traverse(node->left, numThreads);
        }
    }
    #pragma omp taskwait
    process(node);
}

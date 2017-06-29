#include "companytree.h"
#include <omp.h>

void recur(tree *node, int numThreads){
	if(node == NULL)
		return;
	
	node->work_hours = compute_workHours(node->data);
	top_work_hours[node->id] = node->work_hours;
	#pragma omp task
	  recur(node->right, numThreads);
	#pragma omp task
	  recur(node->left, numThreads);
}

void traverse(tree *node, int numThreads) {
	omp_set_num_threads(numThreads);
	#pragma omp parallel
	#pragma omp single
	recur(node, numThreads);
}


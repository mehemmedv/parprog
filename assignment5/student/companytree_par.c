#include "companytree.h"
#include <omp.h>

void traverse(tree *node, int numThreads){	
	
	if (node == NULL)
		return;
	omp_set_num_threads(numThreads);
	omp_set_nested(1);
	omp_set_dynamic(1);
	
	node->work_hours = compute_workHours(node->data);
	top_work_hours[node->id] = node->work_hours;
	#pragma omp parallel
	#pragma omp sections
	{
		#pragma omp section
			traverse(node->right, numThreads);
		#pragma omp section
			traverse(node->left, numThreads);
	}
}




#include "histogram.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "names.h"

pthread_mutex_t mutex[NNAMES];	
int nBlocks;
char *buffer;
int* histgram;
int num_threads;
int len;
int CHUNKSIZEe;

void* get_hist(void *ptr)
{
	int arg = *(int*) ptr;
	char current_word[20] = "";
	int histt[NNAMES];
	for(int i = 0; i < NNAMES; ++i)
		histt[i] = 0;
	bool found = false;
	int yyy = num_threads*CHUNKSIZEe;
	for(int j = arg * CHUNKSIZEe; true ; j += yyy){
		if(found == true)
			break;
		
		//printf("%d %d %d xaxa\n", arg, j, len);
		int c = 0;
		int ttt = j + CHUNKSIZEe;
		for (int i=j; i < ttt; i++) {
			if(buffer[i]==TERMINATOR){
				found = true;
				//printf("%d xaxxaxaaxxa\n", i);
				break;		
			}
			if(isalpha(buffer[i])){
				current_word[c++] = buffer[i];
			} else {
				current_word[c] = '\0';
				int res = getNameIndex(current_word);

				if (res != -1)
					histt[res]++;
				c = 0;
			}
		}
		current_word[c] = '\0';
		int res = getNameIndex(current_word);
		if (res != -1)
			histt[res]++;
	}
	for(int i = 0; i < NNAMES; ++i){
		pthread_mutex_lock (&mutex[i]);
		histgram[i] += histt[i];
		pthread_mutex_unlock (&mutex[i]);
	}
	return NULL;
}

void get_histogram(char *bffr, int* hstgrm, int num_thrds)
{	
	CHUNKSIZEe = CHUNKSIZE;
	buffer = bffr;
	histgram = hstgrm;	
	num_threads = num_thrds;
	len = sizeof(buffer)/sizeof(char);
	int *arg;
	pthread_t * thread;
	thread = ( pthread_t *)  malloc (num_thrds*sizeof(* thread ));
	arg = (int*)malloc(num_thrds*sizeof(*arg));
	for(int i = 0;  i < num_thrds ;  i++) {
		arg [ i ] = i ;
		pthread_create ( thread + i , NULL, &get_hist ,arg+i) ;
	}
	for(int i = 0;  i < num_thrds ;  i++)
		pthread_join ( thread [ i ] , NULL);
	free(thread);
	free(arg) ;

}

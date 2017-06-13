#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "histogram.h"

pthread_mutex_t mutex[NNAMES];	
int nBlocks;
block_t *blocks;
int* histogram;
int num_threads;

void* get_hist(void *ptr)
{
	int arg = *(int*) ptr;
	int histt[NNAMES];
	for(int i = 0; i < NNAMES; ++i)
		histt[i] = 0;
	
	char current_word[20] = "";
	int cnt = nBlocks / num_threads;
	int st = arg*cnt;
	int endd;
	if(arg == num_threads - 1)
		endd = nBlocks;	
	else
		endd = (arg+1)*cnt;

	if(arg != 0){
		int st1 = st - 1;
		int c = 0;		
		bool f = true;
		for(int i = BLOCKSIZE - 20; i < BLOCKSIZE; ++i){
			if(BLOCKSIZE - 20 == i)
				continue;
			c = 0;
			f = true;
			for(int j = i; j < BLOCKSIZE; ++j){
				if(isalpha(blocks[st1][j])){
					current_word[c++] = blocks[st1][j];
				}
 				else{
					f = false;				
					break;			
				}
			}
			if(f == true){
				for(int j = 0; j < BLOCKSIZE - i; ++j){
					if(isalpha(blocks[st][j])){
						current_word[c++] = blocks[st][j];
					}

					// if the character is not an alphabet it is the end of a word.
					// Compare the word with the list of names.
					else {
						current_word[c] = '\0';
						for(int k = 0; k < NNAMES; k++)	{
							if(!strcmp(current_word, names[k])){
								//printf("%d %d xaxa\n", arg, k);						
								//pthread_mutex_lock (&mutex[k]);
								//printf("%d %d\n", k, 1);
								//histogram[k]++;
								//pthread_mutex_unlock (&mutex[k]);
								histt[k]++;
							}
						}
						break;
						c = 0;
					}		
				}
			}		
		}
	}
	

	int c = 0;

	// loop over every character

	for (int i = st; i < endd; i++) {
		for (int j = 0; j < BLOCKSIZE; j++) {

			// if the character is an alphabet store it to form a word.
			if(isalpha(blocks[i][j])){
				current_word[c++] = blocks[i][j];
			}

			// if the character is not an alphabet it is the end of a word.
			// Compare the word with the list of names.
			else {
				current_word[c] = '\0';
				for(int k = 0; k < NNAMES; k++)	{
					if(!strcmp(current_word, names[k])){
						//printf("%d %d xaxa\n", arg, k);						
						//pthread_mutex_lock (&mutex[k]);
						//printf("%d %d\n", k, 1);
//						histogram[k]++;
						//pthread_mutex_unlock (&mutex[k]);
						histt[k]++;
					}
				}
				c = 0;
			}
		}
	}	
	for(int i = 0; i < NNAMES; ++i){
		pthread_mutex_lock (&mutex[i]);
		histogram[i] += histt[i];
		pthread_mutex_unlock (&mutex[i]);
	}

	return NULL;
}


void get_histogram(int nBlks, block_t *blcks, histogram_t hstgrm, int num_thrds)
{	
	//printf("%d %d\n", nBlks, BLOCKSIZE);
	nBlocks = nBlks;
	blocks = blcks;
	histogram = hstgrm;	
	num_threads = num_thrds;

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

#include "histogram.h"
#include "names.h"

#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>  
#include <iostream>

std::thread t[500];

std::mutex mut[NNAMES];

histogram_t histtt;
std::vector<word_t> vvv;
std::vector<word_t>& wordss = vvv;
int num_threads;

void* get_hist(void *ptr)
{
	int id = *(int*) ptr;

	histogram_t hist;
	for(int i = 0; i < NNAMES; ++i)
		hist[i] = 0;

	int len = wordss.size();
	//int ssize = len / num_threads;
	//int st = id * ssize;
	//int endd = std::min(len, (id+1)*ssize);
	//std::vector<word_t>::iterator stth = wordss.begin() + st;
	//std::vector<word_t>::iterator enddth = wordss.end() + endd;
	
	for(int i = id; i < len; i += num_threads){
		
		int resss = getNameIndex(wordss[i].data());

        	if (resss != -1) hist[resss]++;
		//resss = resss;	
	}

	/*for(std::vector<word_t>::iterator i = stth; i != enddth; ++i){
		int resss = getNameIndex((*i).data());
        	//if (resss != -1) hist[resss]++;
		resss = resss;	
	}
	for_each(stth, enddth, [&hist](const word_t& word)
	    {
		int res = getNameIndex(word.data());
		if (res != -1) hist[res]++;
	    });	
	*/

	for(int i = 0; i < NNAMES; ++i){
		mut[i].lock();
		histtt[i] += hist[i];
		mut[i].unlock();
	}
	return NULL;
}

void get_histogram(const std::vector<word_t>& words, histogram_t& histogram, int num_thrds)
{   
    num_threads = num_thrds;
    wordss = words;
//for(word_t ttt : wordss)
//	std::cout<<ttt.data()<<std::endl;
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


    for(int i = 0; i < NNAMES; ++i){
	histogram[i] = histtt[i];
	//std::cout<<" "<<histtt[i]<<std::endl;
	}
}

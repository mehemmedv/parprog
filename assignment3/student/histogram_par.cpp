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
histogram_t res[100];
histogram_t histogramm;
struct data{
	char vvv[64];
	histogram_t hist;
};

void get_hist_par(int id, const std::vector<word_t>& sozler, data* mmm, int len, int num_threads){

	for(int i = 0; i < NNAMES; ++i)
		mmm->hist[i] = 0;

	int ssize = len / num_threads;
	int st = id * ssize;
	int endd = std::min(len, (id+1)*ssize);
	
	for(int it = st; it < endd; ++it){
		int resss = getNameIndex((sozler[it]).data());
		if (resss != -1) mmm->hist[resss]++;
	}

}

void get_histogram(const std::vector<word_t>& word, histogram_t& histogram, int num_thrds)
{
	int len = word.size();
	
    int num_threads = num_thrds;
    struct data* arg = (struct data*)malloc(num_threads*sizeof(*arg));
    for(int i = 0; i < NNAMES; ++i)
	histtt[i] = 0;


    for(int i = 0; i < num_threads; ++i)
	t[i] = std::thread(get_hist_par, i, std::ref(word), arg + i, len, num_threads);	

    for(int i = 0; i < num_threads; ++i)
	t[i].join();

    for(int i = 0; i < NNAMES; ++i)
	for(int j = 0; j < num_threads; ++j)
		histogram[i] += arg[j].hist[i];
}

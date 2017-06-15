#include "histogram.h"
#include "names.h"

#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>  

std::thread t[500];

std::mutex mut[NNAMES];

histogram_t histtt;
std::vector<word_t> ttt;
std::vector<word_t>& words = ttt;
histogram_t res[100][NNAMES];

void get_hist_par(int id, int num_threads){
//	histogram_t hist;
//	for(int i = 0; i < NNAMES; ++i)
//		hist[i] = 0;
	int len = words.size();
	int ssize = len / num_threads;
	int st = id * ssize;
	int endd = std::min(len, (id+1)*ssize);
	std::vector<word_t>::iterator stth = words.begin() + st;
	std::vector<word_t>::iterator enddth = words.begin() + endd;
	
	for(std::vector<word_t>::iterator it = stth; it != enddth; ++it){
		
		int resss = getNameIndex((*it).data());

        	if (resss != -1) hist[resss]++;
		//resss = resss;	
	}

	
	
	for(int i = 0; i < NNAMES; ++i){
		mut[i].lock();
		histtt[i] += hist[i];
		mut[i].unlock();
	}

}

void get_histogram(const std::vector<word_t>& word, histogram_t& histogram, int num_thrds)
{   
    words = word;
    
    for(int i = 0; i < NNAMES; ++i)
	histtt[i] = 0;
    int num_threads = num_thrds;
    for(int i = 0; i < num_threads; ++i)
	t[i] = std::thread(get_hist_par, i, num_threads);
    
    for(int i = 0; i < num_threads; ++i)
	t[i].join();

    for(int i = 0; i < NNAMES; ++i)
	histogram[i] = histtt[i];
}

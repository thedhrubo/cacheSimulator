
/*************************************************************************
/
/ filename: cache.h
/
/ description: This file declares the cache class
/
/ authors: Saha, Dhrubo
/
/ class: CSE 331
/ instructor: Zheng
/ assignment: Lab Project #1
/
/ assigned: Oct 17, 2016
/ due: Oct 31, 2016
/
/************************************************************************/




#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cmath>

#define SIZEOFINT sizeof(int)
class element
{
	public:
		unsigned long int tag;
		unsigned long int time_stored,time_accessed;               
		bool valid;
};


class cache
{
	private:
		int num_sets,data_size,associativity,blockSize,chacheSize,totalChacheBlocks,totalCacheSets,indexbits_tot,tagbits_tot,offsetbits_tot,ramSize,wp,rp,cache_hit_time,cache_miss_penalty, total_instruction_no;
		float n_hit,n_miss, n_load_hit, n_store_hit, n_calc, n_total,n_amat_hit, n_amat_miss, total_instruction, total_hit_rate, load_hit_rate, store_hit_rate,total_load, total_store;		// Maintains the number of cache hits and misses
                int size;
                int mem_capacity;
		unsigned long int timestamp;		// Maintains the time
		element **mem;
                unsigned int* ramAllocation;
                bool fully_associative;
                
	public:
		cache(int *arr);
		bool insert(unsigned long int val);
		bool inCache(unsigned long int val);
                void ramMemmoryAllocation();
                void updateDataRam(int, unsigned int);
		void read_write_request(unsigned long int val,int read_write, int instruction_no);
		float hits();
		float misses();
		unsigned long int time_taken();
		float TRT();		// Total Run Time
		double AMAL();		// Average Memory Access Latency
                float load_hit_rates();
                float store_hit_rates();
                float total_hit_rates();

};

#endif 	//End of file

/*************************************************************************
/
/ filename: cache.cpp
/
/ description: This file contains the necessary functions required for cache execution.
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

#include "../include/cache.h"

cache::cache(int *arr) {
    /* Configuration details */

    blockSize = arr[0];
    associativity = arr[1];
    if (associativity == 0) {
        fully_associative = true;
        associativity = 1;
    }
    data_size = arr[2];
    rp = arr[3];
    cache_miss_penalty = arr[4];
    wp = arr[5];
    cache_hit_time = 1; // cache hit time is default for all the time

    /* ----- */

    /* Initialize the clock (timestamp) and hits/misses to zero */
    timestamp = n_hit = n_miss = 0;
    /* ----- */


    /* Total size of the cache */
    chacheSize = data_size * 1024; // Cache will be in KB so 16KB = 16*1024

    num_sets = (chacheSize / (blockSize*associativity)); // Cache will be in KB so 16KB = 16*1024
    /* ------ */
    //        totalChacheBlocks = chacheSize / blockSize;
    //        totalCacheSets = totalChacheBlocks;        // as the associativity will be direct mapped or fully associative
    //        offsetbits_tot = log2(blockSize);
    //        if(associativity == 1)
    //            indexbits_tot = log2(totalCacheSets);
    //        else indexbits_tot = 0;
    //        tagbits_tot = 32 - (indexbits_tot + offsetbits_tot);
    //        ramSize = ((mem_capacity * 1024 * 1024));
    //        ramMemmoryAllocation();

    mem = new element*[associativity];
    unsigned int i, j;
    for (i = 0; i < associativity; i++) {
        mem[i] = new element[num_sets * blockSize];
    }

    for (i = 0; i < associativity; i++) {
        for (j = 0; j < num_sets; j++)
            mem[i][j].valid = false;
    }
}

void cache::ramMemmoryAllocation() {
    int i;
    ramAllocation = (unsigned int*) malloc(ramSize * 4);

    for (i = 0; i < (int) (ramSize / SIZEOFINT); i++) {
        updateDataRam(i, 0);
    }

}

void cache::updateDataRam(int index, unsigned int data) {
    ramAllocation[index] = data;
}

bool cache::inCache(long unsigned int val) {
    // Increase time
    timestamp++;

    long unsigned int line_val = val % blockSize;
    long unsigned int index = (val / blockSize) % num_sets;
    long unsigned int tag = (val / blockSize) / num_sets;


    /* Calculate the array index where the value needs to be checked */
    long unsigned int arr_index = (index * blockSize) + line_val;
    unsigned long int layer;


    /* Check in the 2-D array for the required tag at the
     * corresponding index in 'each' 1-D array
     */

    for (layer = 0; layer < associativity; layer++) {
        if (mem[layer][arr_index].valid) {
            if (mem[layer][arr_index].tag == tag) {

                //	printf("%lu %lu %lu %lu\n",tag,index,line_val,timestamp);
                mem[layer][arr_index].time_accessed = timestamp;
                return true;
            }
        }
    }

    /* If not found, return false */
    return false;
}

bool cache::insert(unsigned long int val) {
    // Increase time
    timestamp++;

    long unsigned int line_val = val % blockSize;
    long unsigned int index = (val / blockSize) % num_sets;
    long unsigned int tag = (val / blockSize) / num_sets;

    /* Calculate the entry point in the array */

    long unsigned int arr_index = (index * blockSize) + line_val;
    long unsigned int layer;
    //printf("%lu %lu %lu %lu\n",tag,index,line_val,timestamp);
    for (layer = 0; layer < associativity && (mem[layer][arr_index].valid); layer++);
    //std::cerr<<layer<<","<<arr_index<<::endl;
    if (layer != associativity) {
        // If appropriate spot is found, write it there
        mem[layer][arr_index].tag = tag;
        mem[layer][arr_index].valid = true;
        mem[layer][arr_index].time_stored = timestamp;
        mem[layer][arr_index].time_accessed = timestamp;
        return true;
    } else {
        /* If associativityiativity = 1, replace the only place in the memory
         * where it can be entered.
         */

        if (associativity == 1) {
            layer = 0;
            mem[layer][arr_index].tag = tag;
            mem[layer][arr_index].valid = true;
            mem[layer][arr_index].time_stored = timestamp;
            mem[layer][arr_index].time_accessed = timestamp;
            return true;
        }

        /* Replace one of the places on the basis of the replacement policy described*/

        if (rp == 0) {
            /* Random replacement */
            srand(time(NULL));
            int replace_index = rand() % associativity;
            mem[replace_index][arr_index].tag = tag;
            mem[replace_index][arr_index].valid = true;
            mem[replace_index][arr_index].time_stored = timestamp;
            mem[replace_index][arr_index].time_accessed = timestamp;

            return true;
        } else if (rp == 1) {
            /* FIFO replacement
             * Compare the time_stored and replace the one
             * with the minimum value
             */

            unsigned int i, replace_index;
            unsigned long int min_time = 0;
            min_time--; // -1 = MAX of unsigned int
            for (i = 0; i < associativity; i++) {

                if (mem[i][arr_index].time_stored < min_time) {
                    min_time = mem[i][arr_index].time_stored;
                    replace_index = i;
                }

            }

            /* Finally, replace value at the obtained index */
            mem[replace_index][arr_index].tag = tag;
            mem[replace_index][arr_index].valid = true;
            mem[replace_index][arr_index].time_stored = timestamp;
            mem[replace_index][arr_index].time_accessed = timestamp;
            return true;

        } else {
            std::cerr << "Unknown replacement policy" << std::endl;
            exit(2); // 2 => unknown replacement policy
        }


    }


}

void cache::read_write_request(unsigned long int val, int read_write, int instruction_no) {
    /*
     * read_write = 1 (for load)
     * read_write = 2 (for store)
     */

    /* Irrespective of the read_write request, check if the value is already present */
    total_instruction_no = total_instruction_no + instruction_no;
    if (read_write == 1) {
        total_load++;
    } else if (read_write == 2) {
        total_store++;
    } else {
        std::cerr << "Unknown Instruction" << std::endl;
        exit(3); // Unknown write policy
    }
    if (!inCache(val)) {
        n_miss++;
        if (insert(val)) {
            if (read_write == 1)
                return;
            else {
                if (wp == 0) {
                    // Write back, increase time accordingly
                    timestamp = timestamp + 2;
                    return;
                } else if (wp == 1) {
                    // Write through
                    return;
                } else {
                    std::cerr << "Unknown write policy" << std::endl;
                    exit(3); // Unknown write policy
                }
            }
        }
    } else {
        n_hit++;
        //	printf("%lx %lu %d\n",val,val,read_write);
        /* In case of read request, return */
        if (read_write == 1) {
            n_load_hit++;
            return;
        } else {
            n_store_hit++;
            if (wp == 0) {
                // Write back, increase time accordingly
                return;
            } else if (wp == 1) {
                timestamp = timestamp + 2;
                return;
            } else {
                std::cerr << "Unknown write policy" << std::endl;
                exit(3); // Unknown write policy
            }
        }
    }

}

float cache::total_hit_rates() {
    total_instruction = n_hit + n_miss;
    total_hit_rate = (n_hit / total_instruction)*100;
    return total_hit_rate;
}

float cache::load_hit_rates() {
    load_hit_rate = (n_load_hit / total_load)*100;
    return load_hit_rate;
}

float cache::store_hit_rates() {
    store_hit_rate = (n_store_hit / total_store)*100;
    return store_hit_rate;
}

float cache::misses() {
    return n_miss;
}

unsigned long int cache::time_taken() {
    return timestamp;
}

double cache::AMAL() {
    total_instruction = n_hit + n_miss;
    return (total_instruction + (n_miss / total_instruction) * cache_miss_penalty)/total_instruction;
}

float cache::TRT() {
    total_instruction = n_hit + n_miss;
    return (total_instruction * cache_hit_time + (n_miss * cache_miss_penalty) + total_instruction_no);

}

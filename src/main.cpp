/*************************************************************************
/
/ filename: main.cpp
/
/ description: This file contains the main function which taking the file parameter for the cache simulation
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


/* Argument list :-
 *
 * arg1 : input_file
 * arg2 : configuration_file
 */

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "../include/file_reading.h"
#include "../include/cache.h"


using std::string;
using std::ifstream;
using std::ofstream;

int main(int argc, char*args[]) {
    if (argc < 3) {
        // Insufficient number of parameters are passed to the program
        std::cerr << "Please input the input file and the configuration file\n";
        std::cerr << "Usage : cache_simulator <input file> <configuration file>\n";
        exit(1); // exit status 1 : Incorrect number of inputs.
    }

    // If parameters are correct, continue
    std::string abc;
    abc = args[2] + std::string(".out");
    char *output_file = new char[abc.length() + 1];

    std::strcpy(output_file, abc.c_str());
    
    ifstream input(args[2]);
    ifstream conf(args[1]);
    ofstream output(output_file);
    
    
//        ifstream input("trace/twolf.trace");
//        ifstream conf("conf/small-dm.conf");
//        ofstream output("output.txt");

    
    
    int conf_arr[6];

    /* Configuration file is read in this function and the parameters are
     * stored in the 'array' passed.
     *
     * Index        property
     *   0          Line Size
     *   1          associativity
     *   2          Data Size
     *   3          Replacement policy
     *   4          Miss penalty 
     *   5          Write Allocate
     *              Hit time is default 1
     */

    // Set the configuration array by reading the file

    conf_file_reading(conf, conf_arr);

    /* Initialize cache */
    cache *cache_mem = new cache(conf_arr);

    input_file_reading(input, cache_mem, output); // this function is described in file_reading.cpp

    input.close();
    conf.close();
    output.close();
    return 0;
}

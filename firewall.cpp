#include <iostream>
#include <fstream>
#include "BloomFilter.hpp"
#include <cmath> // ceil()
#include <stdint.h>

using namespace std;

/**
 * IMPORTANT! You should use the following lines of code to match the correct output:
 * cout << "This program requires 3 arguments!" << endl;
 * 
 * arg1 - list of malicious urls/bad words filter out
 * arg2 - list of mixed (good/bad) to only write good urls to
 * arg3 - file to write only the good urls to (one on each line)
 */

#define FACTOR 1.5
#define NUM_ARGS 4

/** Obtain the total number of bad URLs read from the file */
unsigned int getNumBadURLs(ifstream& file, string badUrls) {

    string trash;
    unsigned int numBadUrls = 0;

    file.open(badUrls);

    // count number of bad URLs
    while(getline(file, trash))
        ++numBadUrls;

    // reset input file to variable can be reused
    file.close();
    file.seekg(0, ios::beg);

    return numBadUrls;
}

/** Get the size of the file in Bytes */
double getFileSize(ifstream& file, string fileName) {

    file.open(fileName, ios::binary | ios::ate);
    double fileSize = 0;

    /** Because ios::ate goes to the end of the file, tellg()
     *  returns size in Bytes
     */
    if(file) {
        fileSize = file.tellg();
    }

    file.close();
    file.seekg(0, ios::beg);

    return fileSize;
}

// train bloom filter and classify set of unknown urls as safe or not
int main(int argc, char** argv) {

    // check for correct number of aruments
    if(argc != NUM_ARGS) {
        cout << "This program requires 3 arguments!" << endl;
        return -1;
    }

    ifstream file;   // for reading all input files
    ifstream check;
    ofstream output; // output file
    string url;
    string badUrls = argv[1];
    string goodUrls = argv[2];
    string outputFile = argv[3];

    double posRate = 0;     // hold false positive rate
    double memRatio = 0;    // hold memory saving ratio
    double numBadUrls = 0;  // hold number of bad URLs
    double numSafeUrls = 0; // hold number of safe URLs
    double numUrls = 0;     // total number of URLs read
    double numOutput = 0;   // number of URLs outputted to file
    double numBytes = 0;
    double fileSize = 0;

    numBadUrls = getNumBadURLs(file, badUrls);
    numBytes = ceil(FACTOR*numBadUrls); // calculate max space for hash table


    /** train bloom filter, classify set of unknown urls, and output "safe" one
     *  to an output file
     */
    BloomFilter filter(numBytes);
    filter.trainFilter(file, badUrls, filter);
    filter.processURLs(file, goodUrls, output, outputFile, numOutput, numUrls);

    // print statistics
    fileSize = getFileSize(file, badUrls);
    numSafeUrls = numUrls - numBadUrls;
    posRate = (numSafeUrls - numOutput)/numSafeUrls;
    memRatio = fileSize / numBytes;
    cout << "False positive rate: " << posRate << endl;
    cout << "Saved memory ratio: " << memRatio << endl;

    return 0;
}

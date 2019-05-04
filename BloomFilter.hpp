#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP

#include <string>
#include <fstream>
#include "MurmurHash3.h"
#include <stdint.h>

using namespace std;

/**
 * The class for bloom filter that provides memory efficient check
 * of whether an item has been inserted before. Small amount of 
 * false positives is possible but there will be no false negatives.
 */
class BloomFilter {

public:

    /* Create a new bloom filter with the size in bytes */
    BloomFilter(uint64_t numBytes);

    /* Insert an item into the bloom filter */
    void insert(std::string item);

    /* Determine whether an item is in the bloom filter */
    bool find(std::string item);

    void trainFilter(ifstream& file, string badUrls, BloomFilter& filter);

    void processURLs(ifstream& file, string goodUrls, ofstream& output,
            string outputFile, double& numOutput, double& numUrls);

    /* Destructor for the bloom filter */
    ~BloomFilter();

private:



    // The char array as the hash table
    unsigned char* table;
    uint64_t tableSize;
    void setBit(uint64_t pos);
    bool hasBit(uint64_t pos);

    // TODO: Add your own data and method here

};
#endif // BLOOM_FILTER

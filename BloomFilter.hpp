/**
 * Filename:     BloomFilter.hpp
 *
 * Team:         Brandon Olmos (bolmos@ucsd.edu),
 *               Daryl Nakamoto (dnakamot@ucsd.edu)
 *
 * Reference(s): cplusplus.com
 *           +++ https://github.com/aappleby/smhasher/wiki/MurmurHash3
 *
 * Description:  Bloom filter used to make predictions on mixed unknown urls
 */

#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP

#include <fstream>
#include <string>
#include <stdint.h>
#include "MurmurHash3.h" // See +++ above

using namespace std;

/**
 * The class for bloom filter that provides memory efficient check
 * of whether an item has been inserted before. Small amount of 
 * false positives is possible but there will be no false negatives.
 */
class BloomFilter {

private:

    // create has table using char array. The bits are the spots
    unsigned char* table;
    uint64_t tableSize;


    /** insert in pos position of hash table */
    void setBit(unsigned int pos);

    /** check if pos position in hash table is filled */
    bool hasBit(unsigned int pos);

public:

    /** Destructor for the bloom filter */
    ~BloomFilter();

    /** Create a new bloom filter with the size in bytes */
    BloomFilter(uint64_t numBytes);

    /** Insert an item into the bloom filter */
    void insert(std::string item);

    /** Determine whether an item is in the bloom filter */
    bool find(std::string item);

    /** train bloom filter */
    void trainFilter(ifstream& file, string badUrls, BloomFilter& filter);

    /** read file of urls and write good urls to an output file */
    void processURLs(ifstream& file, string goodUrls, ofstream& output,
            string outputFile, double& numOutput, double& numUrls);

};
#endif // BLOOM_FILTER

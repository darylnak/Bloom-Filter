/**
 * Filename:     BloomFilter.cpp
 *
 * Team:         Brandon Olmos (bolmos@ucsd.edu),
 *               Daryl Nakamoto (dnakamot@ucsd.edu)
 *
 * Reference(s): cplusplus.com
 *               https://github.com/aappleby/smhasher/wiki/MurmurHash3
 *
 * Description:  Bloom filter used to make predictions on mixed unknown urls
 *               Provides memory efficient check of whether an item has
 *               been inserted before. Small amount of false positives is
 *               possible but there will be no false negatives.
 */
 */

#include "BloomFilter.hpp"
#include <iostream>
#include <cmath>
#include <cstring>


#define SEED1 3
#define SEED2 5
#define SEED3 7

using namespace std;

/** Create a new bloom filter with the size in bytes */
BloomFilter::BloomFilter(uint64_t numBytes)
{
    table = new unsigned char[numBytes];
    tableSize = numBytes * 8; // 8 bits per Byte

    // set all bits in table to 0
    memset(table, 0, numBytes);
}

/** Destructor for the bloom filter */
BloomFilter::~BloomFilter()
{
    delete table;
}

/** insert in pos position of hash table */
void BloomFilter::setBit(unsigned int pos) {

    unsigned index = pos / 8;     // select char
    unsigned bitInd = pos % 8;    // select position in char

    // go to the pos bit in table
    bitInd = pow(2, bitInd);

    // set the pos bit
    table[index] = table[index] | bitInd;
}

/** check if pos position in hash table is filled */
bool BloomFilter::hasBit(unsigned int pos) {

    unsigned int index = pos / 8;    // select char
    unsigned int bitInd = pos % 8;   // select position in char

    // go to the pos bit
    bitInd = pow(2, bitInd);

    // return true if the bit has been set, false if not
    return table[index] & bitInd;
}

/** train bloom filter */
void BloomFilter::trainFilter(ifstream& file, string badUrls,
        BloomFilter& filter) {

    string url;

    // read bad urls and train filter
    file.open(badUrls);
    while(getline(file, url))
        filter.insert(url);

    // reset file stream so variable can be reused
    file.close();
    file.seekg(0, ios::beg);
}

/** read file of urls and write good urls to an output file */
void BloomFilter::processURLs(ifstream& file, string goodUrls, ofstream& output,
        string outputFile, double& numOutput, double& numUrls) {

    string url;
    bool isBadURL;

    // read the file and write predicted good urls to output file, outputFile
    file.open(goodUrls);
    output.open(outputFile);
    while(getline(file, url)) {
        isBadURL = find(url);

        if(!isBadURL) {
            output << url << endl;
            ++numOutput;
        }

        ++numUrls;
    }

    // close and clear the buffers for both file so they can be reused later
    file.close();
    file.seekg(0, ios::beg);
    output.close();
    output.seekg(0, ios::beg);
}

/** Insert an item into the bloom filter */
void BloomFilter::insert(string item)
{
    // hold the hash value returned from hash function
    uint64_t output[2];

    // hold positions returned from hash functions
    uint64_t pos1;
    uint64_t pos2;
    uint64_t pos3;

    // get url from input (requirement of Murmurhash3; has to be this way)
    char str[item.size() + 1];
    strcpy(str, item.c_str());


    /** Get 3 hash values to set bits for item */
    MurmurHash3_x64_128(&str, sizeof(str), uint64_t(SEED1), output);
    pos1 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), uint64_t(SEED2), output);
    pos2 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), uint64_t(SEED3), output);
    pos3 = output[1] % tableSize;

    // set the bits
    setBit(pos1);
    setBit(pos2);
    setBit(pos3);
}

/** Determine whether an item is in the bloom filter */
bool BloomFilter::find(string item)
{
    // hold the hash value returned from hash function
    uint64_t output[2];

    // hold positions returned from hash functions
    uint64_t pos1 = 0;
    uint64_t pos2 = 0;
    uint64_t pos3 = 0;

    char str[item.size() + 1];
    strcpy(str, item.c_str());


    /** Get the 3 hash values used to set the bits for item */
    MurmurHash3_x64_128(&str, sizeof(str), uint64_t(SEED1), output);
    pos1 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), uint64_t(SEED2), output);
    pos2 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), uint64_t(SEED3), output);
    pos3 = output[1] % tableSize;

    // check if item was inserted into table (make prediction)
    if(hasBit(pos1) && hasBit(pos2) && hasBit(pos3))
        return true;

    return false;
}
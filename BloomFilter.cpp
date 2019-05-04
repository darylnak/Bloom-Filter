#include "BloomFilter.hpp"
#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

/* Create a new bloom filter with the size in bytes */
BloomFilter::BloomFilter(uint64_t numBytes)
{
    table = new unsigned char[numBytes];
    tableSize = numBytes * 8;

    memset(table, 0, numBytes);
}

/* Destructor for the bloom filter */
BloomFilter::~BloomFilter()
{
    if(table)
        delete table;
}

void BloomFilter::setBit(uint64_t pos) {
    // select char
    uint64_t index = pos / 8;

    // hold index for bit in char
    uint64_t bitInd = pos % 8;

    // go to the pos bit
    bitInd = pow(2, bitInd);

    // change the pos bit
    table[index] = table[index] | bitInd;
}

bool BloomFilter::hasBit(uint64_t pos) {
    // select char
    uint64_t index = pos / 8;

    // hold index for bit in char
    uint64_t bitInd = pos % 8;

    // go to the pos bit
    bitInd = pow(2, bitInd);

    // change the pos bit
    return table[index] & bitInd;
}

void BloomFilter::trainFilter(ifstream& file, string badUrls, BloomFilter& filter) {

    string url;

    file.open(badUrls);

    while(getline(file, url))
        filter.insert(url);

    file.close();
    file.seekg(0, ios::beg);
}

void BloomFilter::processURLs(ifstream& file, string goodUrls, ofstream& output,
        string outputFile, double& numOutput, double& numUrls) {

    bool isBadURL;
    string url;

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
    file.close();
    output.close();
}

/* Insert an item into the bloom filter */
void BloomFilter::insert(string item)
{
    uint32_t seed3 = 3;
    uint32_t seed5 = 5;
    uint32_t seed7 = 7;

    uint64_t output[2];
    uint64_t pos1;
    uint64_t pos2;
    uint64_t pos3;
    char str[item.size() + 1];

    strcpy(str, item.c_str());

    MurmurHash3_x64_128(&str, sizeof(str), seed3, output); // hash function
    pos1 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), seed5, output); // hash function
    pos2 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), seed7, output); // hash function
    pos3 = output[1] % tableSize;

    setBit(pos1);
    setBit(pos2);
    setBit(pos3);
}

/* Determine whether an item is in the bloom filter */
bool BloomFilter::find(string item)
{
    uint32_t seed3 = 3;
    uint32_t seed5 = 5;
    uint32_t seed7 = 7;

    uint64_t output[2];
    uint64_t pos1 = 0;
    uint64_t pos2 = 0;
    uint64_t pos3 = 0;
    char str[item.size() + 1];

    strcpy(str, item.c_str());

    MurmurHash3_x64_128(&str, sizeof(str), seed3, output); // hash function
    pos1 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), seed5, output); // hash function
    pos2 = output[1] % tableSize;

    MurmurHash3_x64_128(&str, sizeof(str), seed7, output); // hash function
    pos3 = output[1] % tableSize;

    if(hasBit(pos1) && hasBit(pos2) && hasBit(pos3))
        return true;

    return false;
}
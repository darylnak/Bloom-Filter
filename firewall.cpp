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

unsigned int getNumBadURLs(ifstream& file, string badUrls) {

    string trash;
    unsigned int numBadUrls = 0;

    file.open(badUrls);

    while(getline(file, trash))
        ++numBadUrls;

    file.close();
    file.seekg(0, ios::beg);

    return numBadUrls;
}

double getFileSize(ifstream& file, string fileName) {

    file.open(fileName, ios::binary | ios::ate);
    double fileSize = 0;

    if(file) {
        fileSize = file.tellg();
    }

    file.close();
    file.seekg(0, ios::beg);

    return fileSize;
}

int main(int argc, char** argv) {

    // correct number of aruments
    if(argc != NUM_ARGS) {
        cout << "This program requires 3 arguments!" << endl;
        return -1;
    }

    ifstream file;
    ifstream check;
    ofstream output;
    string url;
    string badUrls = argv[1];
    string goodUrls = argv[2];
    string outputFile = argv[3];

    double posRate = 0;
    double memRatio = 0;
    double numBadUrls = 0;
    double numSafeUrls = 0;
    double numUrls = 0;
    double numOutput = 0;
    double numBytes = 0;
    double fileSize = 0;

    numBadUrls = getNumBadURLs(file, badUrls);
    numBytes = ceil(FACTOR*numBadUrls);


    BloomFilter filter(numBytes);
    filter.trainFilter(file, badUrls, filter);
    filter.processURLs(file, goodUrls, output, outputFile, numOutput, numUrls);


    fileSize = getFileSize(file, badUrls);
    numSafeUrls = numUrls - numBadUrls;
    posRate = (numSafeUrls - numOutput)/numSafeUrls;
    memRatio = fileSize / numBytes;
    cout << "False positive rate: " << posRate << endl;
    cout << "Saved memory ratio: " << memRatio << endl;

    return 0;
}

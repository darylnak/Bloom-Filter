#include <iostream>
#include "DictionaryTrie.hpp"

using namespace std;

/**
 * IMPORTANT! You should use the following lines of code to match the correct output:
 * 
 * cout << "This program needs exactly one argument!" << endl;
 * cout << "Reading file: " << file << endl;
 * cout << "Enter a prefix/pattern to search for:" << endl;
 * cout << "Enter a number of completions:" << endl;
 * cout << completion << endl;
 * cout << "Continue? (y/n)" << endl;
 * 
 * arg 1 - Input file name (in format like freq_dict.txt)
 */
int main(int argc, char** argv)
{
    // incorrect argument
    if(argc != 2) {
        cout << "This program needs exactly one argument!" << endl;
        return -1;
    }

    string file = argv[1];
    string prefix;
    string check;
    int numCompletions;
    char _continue;

    cout << "Reading file: " << file << endl;
    /**
     * READ FILE
     */

    do {
        cout << "Enter a prefix/pattern to search for:" << endl;
        cin >> prefix;


        cout << "Enter a number of completions:" << endl;
        cin >> check;


        // DEBUGGING - check input values
//    cout << prefix << endl;
//    cout << numCompletions << endl;

        /**
         * DO AUTOCOMPLETION
         */

        cout << "Continue? (y/n)" << endl;
        cin >> _continue;
    } while(_continue == 'y');

    return 0;
}

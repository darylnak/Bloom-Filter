/**
 * Filename:     autocomplete.cpp
 *
 * Team:         Brandon Olmos (bolmos@ucsd.edu),
 *               Daryl Nakamoto (dnakamot@ucsd.edu)
 *
 * Reference(s): cplusplus.com
 *
 * Description:  Read a dictionary into ternary trie and suggest
 *               autocompletion K results to users based on an input prefx
 *               and K number of wanted suggested completions.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "DictionaryTrie.hpp"
#include "util.hpp"

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
    /** DEBUGGING *

    string test = "hello";
    string hold;

    while(test.length() > 0)
    {
        cout << test << endl;
        test.erase(0, 1);
    }

    DictionaryTrie trie;
    DictionaryTrie trie2;

    bool check1 = false;


    check1 = trie.insert("g", 10);
    check1 = trie.insert("a", 1);
    check1 = trie.insert("h", 1000);
    check1 = trie.insert("g", 10);
    check1 = trie.insert("a", 1);
    check1 = trie.insert("h", 1000);

    check1 = trie2.insert("get", 30000);
    check1 = trie2.insert("apple", 1);
    check1 = trie2.insert("gat", 2);
    check1 = trie2.insert("git", 3);
    check1 = trie2.insert("git", 3);
    check1 = trie2.insert("github", 1337);

    //*/

    // exit  due to incorrect argument count
    if(argc != 2) {
        cout << "This program needs exactly one argument!" << endl;
        return -1;
    }

    string file = argv[1];          // get file name
    string prefix;                  // hold prefix
    string getCompletions;          // hold number of completions
    unsigned int numCompletions;    //
    char _continue;
    Utils read;
    DictionaryTrie dictionary;
    vector<string> completions;
    ifstream load;

    cout << "Reading file: " << file << endl;

    load.open(file, ifstream::in);

    read.load_dict(dictionary, load);

    load.close();


    // start program
    do {
        cout << "Enter a prefix/pattern to search for:" << endl;
        getline(cin, prefix);

        cout << "Enter a number of completions:" << endl;
        cin >> getCompletions;

        istringstream convert(getCompletions);

        convert >> numCompletions;

        completions = dictionary.predictCompletions(prefix, numCompletions);

        for(auto itr = completions.begin(); itr != completions.end(); ++itr)
            cout << *itr << endl;

        cout << "Continue? (y/n)" << endl;
        cin >> _continue;

        cin.ignore();

    } while(_continue == 'y');

    return 0;
}

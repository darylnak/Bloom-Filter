/**
 * Filename:     TNode/hpp
 *
 * Team:         Brandon Olmos (bolmos@ucsd.edu),
 *               Daryl Nakamoto (dnakamot@ucsd.edu)
 *
 * Reference(s): cplusplus.com
 *
 * Description:  Node class for Dictionary. Used for ternery trie to hold
 *               a char in word and frequency of word.
 */

#ifndef TNODE_HPP
#define TNODE_HPP


using namespace std;

class TNode {

public:

    TNode* left;
    TNode* right;
    TNode* middle;
    char _char;
    unsigned int freq;
    int fmid;
    int fright;
    int fleft;


    /** Default constructor for TNode*/
    TNode() {
        left = right = middle = nullptr;
        freq = 0;
        fmid = fright = fleft  = 0;
    }

    /** Make a new TNode with char c*/
    TNode(char& c) {
        left = right = middle = nullptr;
        freq = 0;
        _char = c;
        fmid = fright = fleft  = 0;
    }
};

#endif //TNODE_HPP
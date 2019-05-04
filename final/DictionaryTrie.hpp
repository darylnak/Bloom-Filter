/**
 * Filename:     DictionaryTrie.hpp
 *
 * Team:         Brandon Olmos (bolmos@ucsd.edu),
 *               Daryl Nakamoto (dnakamot@ucsd.edu)
 *
 * Reference(s): cplusplus.com
 *
 * Description:  Ternery trie to hold strings and their corresponding
 *               occuring frequency. Suggest n most probable word completions
 *               based on frequency of words with a particular prefix.
 */

#ifndef DICTIONARYTRIE_HPP
#define DICTIONARYTRIE_HPP

#include <vector>
#include <string>
#include <queue>
#include "TNode.hpp"

#define EMPTYSTR ""
typedef pair<string, int> Word;

using namespace std;
/**
 *  The class for a dictionary ADT, implemented as a ternery trie
 */


class DictionaryTrie
{
private:

    /** Comparator struct for TNodes. (priority queue)*/
    struct Compare {
    public:
        bool operator()(const Word& str1, const Word& str2)
        {
            if(str1.second == str2.second)
                return str1.first > str2.first;

            return str1.second < str2.second;
        }
    };

    priority_queue<Word, vector<Word>, Compare> numComplete;
    TNode* root;

    /** predictCompletions helper function; return ALL suggestions */
    void getCompletions(string word, TNode* curr) {

        // naive post-order traversal to get all words for a given prefix

        if(curr == nullptr) return;

        if(topResults.size() < numResults || curr->fleft >= threshold)
            getCompletions(word, curr->left);

        if(topResults.size() < numResults || curr->fright >= threshold)
            getCompletions(word, curr->right);

        if(curr->middle && (topResults.size() < numResults || curr->fmid >= threshold))
            getCompletions(word + curr->_char, curr->middle);


        // insert if a word
        if((topResults.size() < numResults && curr->freq > 0) || curr->freq > threshold) {
            numComplete.push(Word(word + curr->_char, curr->freq));

            topResults.insert(topResults.begin(), curr->freq);

            if(topResults.size() < numResults + 2)
                sort(topResults.begin(), topResults.end());
            else
                sort(topResults.begin(), topResults.begin() + numResults + 1);

            if(topResults.size() >= numResults)
                threshold = topResults[0];
            else if((topResults.size() < numResults && curr->freq < threshold) || threshold == 0)
                threshold = curr->freq;

        }

    }

    unsigned int threshold;
    unsigned int numResults;
    vector<int> topResults;

public:

  /** Create a new Dictionary that uses a Trie back end */
  DictionaryTrie() : root(nullptr), threshold(0) {}

/**
 * Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string). This might be useful for testing
 * when you want to test a certain case, but don't want to
 * write out a specific word 300 times.
 */
    bool insert(string word, unsigned int freq)
    {

        if (word == "git her")
            cout << " sd";

        // reject empty string
        if(word == EMPTYSTR) return false;

        TNode* curr;
        int index = 0;
        int length = word.length();

        // single character word AND initial build (empty trie)
        if(root == nullptr) {

            root = new TNode(word[index]);

            if(length == 1) {

                root->freq = freq;
                return true;
            }
        }

        curr = root;

        // start insertion
        while(index < length) {

            // check middle child
            if(curr->_char == word[index])
            {

                if(curr->middle == nullptr)
                {
                    // reject duplicate (no middle child)
                    if(index + 1 == length) return false;

                    else
                    {
                        curr->middle = new TNode(word[index + 1]);

                        // done inserting word
                        if(index + 1 == length - 1)
                        {
                            if(curr->fmid < freq) curr->fmid = freq;
                            curr->middle->freq = freq;
                            return true;
                        }
                    }
                }

                if(index + 1 == length && curr->freq == 0) {
                    if(curr->fmid < freq) curr->fmid = freq;
                    curr->freq = freq;
                    return true;
                }


                // check next character in string
                if(curr->fmid < freq) curr->fmid = freq;
                curr = curr->middle;
                index++;
            }

            // check left child
            else if(word[index] < curr->_char)
            {

                if(curr->left == nullptr)
                {
                    // create new left node if necessary
                    curr->left = new TNode(word[index]);

                    if (index + 1 == length) {
                        if(curr->fleft < freq) curr->fleft = freq;
                        curr->left->freq = freq;
                        return true;
                    }
                }

                if(index + 1 == length && curr->freq == 0) {
                    if(curr->fleft < freq) curr->fleft = freq;
                    curr->freq = freq;
                    return true;
                }

                if(curr->fleft < freq) curr->fleft = freq;
                curr = curr->left;
            }

            // check right child
            else
            {

                if(curr->right == nullptr)
                {
                    curr->right = new TNode(word[index]);

                    if(index + 1 == length)
                    {
                        if(curr->fright < freq) curr->fright = freq;
                        curr->right->freq = freq;
                        return true;
                    }
                }

                if(index + 1 == length && curr->freq == 0) {
                    if(curr->fright < freq) curr->fright = freq;
                    curr->freq = freq;
                    return true;
                }

                if(curr->fright < freq) curr->fright = freq;
                curr = curr->right;
            }

        }

        // reject duplicate
        return false;
    }

  /** Return true if word is in the dictionary, and false otherwise.
   */
  bool find(string word) const
  {

      if(word == EMPTYSTR || root == nullptr) return false;

      TNode* curr = root;
      int index = 0;
      int length = word.length();

      // simply, perform trinary search
      while(index < length)
      {
          // check middle and check if word
          if(curr->_char == word[index])
          {

              index++;

              if(index == length && curr->freq > 0) return true;

              else if(curr->middle == nullptr) return false;

              else curr = curr->middle;
          }

          // check left
          else if(word[index] < curr->_char)
          {
              if(curr->left == nullptr) return false;

              else curr = curr->left;
          }


          // check right
          else
          {
              if(curr->right == nullptr) return false;

              else curr = curr->right;
          }
      }

      return false;
  }


  /** Return up to num_completions of the most frequent completions
   * of the prefix, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than num_completions legal completions, this
   * function returns a vector with as many completions as possible.
   * If no completions exist, then the function returns a vector of size 0.
   * The prefix itself might be included in the returned words if the prefix
   * is a word (and is among the num_completions most frequent completions
   * of the prefix)
   */
  vector<string> predictCompletions(string prefix, unsigned int num_completions)
  {
      numComplete = priority_queue<Word, vector<Word>, Compare>();
      vector<string> mostFreqStr = vector<string>();
      numResults = num_completions;
      threshold = 0;
      topResults = vector<int>();

      TNode* curr = root;
      int index = 0;
      int preLength = prefix.length();

      // no completion suggestions
      if(num_completions == 0) return  mostFreqStr;

      // go to prefix position
      while(curr && index < preLength)
      {
          if(curr->_char == prefix[index])
          {
              index++;

              if(index != preLength) curr = curr->middle;
          }

          else if(prefix[index] < curr->_char)
          {
              curr = curr->left;
          }

          else
          {
              curr = curr->right;
          }
      }

      // prefix not found
      if(curr == nullptr) return mostFreqStr;

      // if prefix is a word, add to queue
      if(curr->freq > 0)
          numComplete.push(Word(prefix, curr->freq));

      // start autocompletion
      if(curr->middle)
        getCompletions(prefix, curr->middle);

      unsigned int i = 0;
      while(numComplete.size() != 0 && i < num_completions) {
          mostFreqStr.push_back(numComplete.top().first);
          numComplete.pop();
          ++i;
      }

      return mostFreqStr;

  }

  /**Return up to num_completions of the most frequent completions
   * of the pattern, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than num_completions legal completions, this
   * function returns a vector with as many completions as possible.
   * If no completions exist, then the function returns a vector of size 0.
   * The pattern itself might be included in the returned words if the pattern
   * is a word (and is among the num_completions most frequent completions
   * of the pattern)
   */
  vector<string> predictUnderscore(string pattern, unsigned int num_completions){
      vector<string> empty;

      return empty;
  }

  /** Destructor */
  ~DictionaryTrie() {
     deleteTree(root);
  }

  /** post-order traversal to delete trie */
  void deleteTree(TNode* curr) {
      if(curr == nullptr) return;

      if(curr->left) deleteTree(curr->left);

      if(curr->middle) deleteTree(curr->middle);

      if(curr->right) deleteTree(curr->right);

      delete curr;
  }
};

#endif // DICTIONARYTRIE_HPP

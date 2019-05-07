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

    TNode* root;

    // number of suggestions to return
    priority_queue<Word, vector<Word>, Compare> numComplete;

    /** PredictCompletions helper function; return ALL suggestions.
     *  Returns the frequency of the most frequent words in order.
     *  Traversed in order, so traversed in alphanumerical order.
     *  This is why the words can be directly inserted into a vector,
     *  rather than a priority queue.
     */
    int getCompletions(string& word, TNode*& curr, int& highestFreq,
            vector<string>& numComplete, unsigned int& num_completions) {

        // remember next best frequency and result from a recursive call
        int nextBest = 0;
        int recResult = 0;

        // stop search if obtained number of completions requested
        if(numComplete.size() < num_completions) {

            /** Check if the current highest frequency word was found.
             *  Push into the return vector if so.
             */
            if (curr->freq == highestFreq) {
                word.push_back(curr->_char);
                numComplete.push_back(word);
                word.pop_back();
            }

            // Search for words to the left and return next best if it was left
            if (curr->left) {
                if (curr->fleft >= highestFreq) {
                    recResult = getCompletions(word, curr->left, highestFreq,
                                               numComplete, num_completions);

                    // check if found a better next frequency
                    if (recResult > nextBest) nextBest = recResult;
                } else if (curr->fleft > nextBest)
                    nextBest = curr->fleft;
            }

            // Search for words to the mid and return next best if it was mid
            if (curr->middle) {
                if (curr->fmid >= highestFreq) {
                    word.push_back(curr->_char);
                    recResult = getCompletions(word, curr->middle, highestFreq,
                                               numComplete, num_completions);
                    word.pop_back();

                    // check if found a better next frequency
                    if (recResult > nextBest) nextBest = recResult;
                } else if (curr->fmid > nextBest)
                    nextBest = curr->fmid;
            }

            //Search for words to the right and return next best if it was right
            if (curr->right) {
                if (curr->fright >= highestFreq) {
                    recResult = getCompletions(word, curr->right, highestFreq,
                                               numComplete, num_completions);

                    // check if found a better next frequency
                    if (recResult > nextBest) nextBest = recResult;
                } else if (curr->fright > nextBest)
                    nextBest = curr->fright;
            }

            /** check if a next best was found. Will be false no more words with
            *   prefix
            */
            if (curr->freq > nextBest && curr->freq < highestFreq)
                nextBest = curr->freq;

        }

        return nextBest;
    }

    /** find most frequent words with wildcard (exact length of input) */
    int postUnderscore(string postUnderscore,TNode* node) {

        // check for empty input or empty tree
        if(postUnderscore == EMPTYSTR || root == nullptr) return 0;

        TNode* curr = node;
        int index = 0;
        int length = postUnderscore.length();

        // perform trinary search
        while(index < length)
        {
            // check middle and check if word
            if(curr->_char == postUnderscore[index])
            {
                index++;

                if(index == length && curr->freq > 0) return curr->freq;

                else if(curr->middle == nullptr) return 0;

                else curr = curr->middle;
            }

            // check left
            else if(postUnderscore[index] < curr->_char)
            {
                if(curr->left == nullptr) return 0;

                else curr = curr->left;
            }


            // check right
            else
            {
                if(curr->right == nullptr) return 0;

                else curr = curr->right;
            }
        }

        return 0;
    }

    /** Get all children the left and right of curr
     *  Used for pre underscore to fill underscore.
     *  All potential letters to be in word will be to the left or right
     */
    void getChildren(TNode* curr, queue<TNode*>& children) {

        if(curr == nullptr) return;

        getChildren(curr->left, children);

        children.push(curr);

        getChildren(curr->right, children);
    }

public:

  /** Create a new Dictionary that uses a Trie back end */
  DictionaryTrie() : root(nullptr) {}

/**
 * Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string). This might be useful for testing
 * when you want to test a certain case, but don't want to
 * write out a specific word 300 times.
 */
    bool insert(string word, int freq)
    {

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

            if(index + 1 == length && curr->freq > 0 && curr->_char  == word[index]) return false;

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
                            if(freq > curr->fmid) curr->fmid = freq;
                            curr->middle->freq = freq;
                            return true;
                        }
                    }
                }

                // if inserting as a substring of another string
                if(index + 1 == length && curr->freq == 0) {
                    curr->freq = freq;
                    return true;
                }

                // check next character in string
                if(freq > curr->fmid) curr->fmid = freq;
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
                        if(freq > curr->fleft) curr->fleft = freq;
                        curr->left->freq = freq;
                        return true;
                    }
                }

                if(freq > curr->fleft) curr->fleft = freq;
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
                        if(freq > curr->fright) curr->fright = freq;
                        curr->right->freq = freq;
                        return true;
                    }
                }

                if(freq > curr->fright) curr->fright = freq;
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
      vector<string> mostFreqStr;
      mostFreqStr.reserve(num_completions);

      TNode* curr = root;
      int index = 0;
      int preLength = prefix.length();


      prefix.reserve(100);
      string& str = prefix;

      // no completion suggestions
      if(!num_completions) return mostFreqStr;

      // go to prefix position
      while(curr && index < preLength)
      {
          if(curr->_char == prefix[index])
          {
              index += 1;
              if(index != preLength) curr = curr->middle;
          }

          else if(prefix[index] < curr->_char)
              curr = curr->left;

          else
              curr = curr->right;
      }

      // prefix not found
      if(!curr) return mostFreqStr;

      int prefixFreq = curr->freq;

      // push to queue if prefix is largest word
      int max = curr->fmid;

      if(!max && prefixFreq)
          mostFreqStr.push_back(str);

      // obtain num_completions completions
      while(mostFreqStr.size() < num_completions && max) {

          if(prefixFreq >= max) {
              mostFreqStr.push_back(str);
              prefixFreq = 0;
          }

          max = getCompletions(str, curr->middle, max, mostFreqStr,
                  num_completions);
      }
//
//      /** Obtain num_completions. To continue above while loop, but without
//       *  the if statement to improve time efficiency
//       */
//      while(mostFreqStr.size() < num_completions && max) {
//          max = getCompletions(prefix, curr->middle, max, mostFreqStr,
//                  num_completions);
//      }

      // return suggestions
      return mostFreqStr;
  }

  /* Return up to num_completions of the most frequent completions
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

      priority_queue<Word, vector<Word>, Compare> numComplete;
      vector<string> mostFreqStr = vector<string>();
      queue<TNode*> preUnderscore = queue<TNode*>();

      TNode* curr = root;
      int index = 0;
      int preLength = pattern.length();
      char underscore = '_';
      int freq = 0;

      // no completion suggestions
      if(num_completions == 0) return  mostFreqStr;

      // go to pre-underscore position
      while(curr && index < preLength && pattern[index] != underscore)
      {
          if(curr->_char == pattern[index])
          {
              index++;

              if(index != preLength) curr = curr->middle;
          }

          else if(pattern[index] < curr->_char)
          {
              curr = curr->left;
          }

          else
          {
              curr = curr->right;
          }
      }

      getChildren(curr, preUnderscore);

      // underscore at end
      if(pattern[pattern.length() - 1] == underscore) {
          while(preUnderscore.size()) {
              curr = preUnderscore.front();

              if(curr->freq > 0) {
                  pattern[index] = curr->_char;
                  numComplete.push(Word(pattern, curr->freq));
              }

              preUnderscore.pop();

          }
      }

      // make better
      while(pattern[pattern.length() - 1] != underscore && preUnderscore.size()) {
          curr = preUnderscore.front();

          if(curr->middle)
            freq = postUnderscore(pattern.substr(index + 1, pattern.length()),curr->middle);

          if(freq) {
              pattern[index] = curr->_char;
              numComplete.push(Word(pattern, freq));
          }

          preUnderscore.pop();
      }

      unsigned int i = 0;
      while(numComplete.size() != 0 && i < num_completions) {
          mostFreqStr.push_back(numComplete.top().first);
          numComplete.pop();
          ++i;
      }

      return mostFreqStr;

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

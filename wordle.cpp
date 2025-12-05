#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include <vector>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
    const std::string& pattern,
    const std::set<std::string>& dict,
    const std::set<std::string>& prefixes,
    std::string& current,
    std::string floating,
    size_t idx,
    const std::vector<int>& blanksRem,
    std::set<std::string>& results);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results;
    
    if (in.empty()){
        return results;
    }

    size_t len = in.size();

    std::vector<int> blanksRem(len + 1, 0);
    int i = (int)len - 1;
    while(i >= 0) {                      // loop #1
        blanksRem[i] = blanksRem[i + 1];
        if(in[i] == '-') {
            blanksRem[i] = blanksRem[i] + 1;
        }
        i = i - 1;
    }

    if((int)floating.size() > blanksRem[0]) {
        return results;
    }

    std::set<std::string> prefixes;
    std::set<std::string>::const_iterator it = dict.begin();
    while(it != dict.end()) {            // loop #2
        const std::string& w = *it;
        if(w.size() == len) {
            std::string pref;
            size_t j = 0;
            while(j < w.size()) {        // loop #3
                pref.push_back(w[j]);
                prefixes.insert(pref);
                j = j + 1;
            }
        }
        ++it;
    }

    std::string current = in;
    wordleHelper(in, dict, prefixes, current, floating, 0, blanksRem, results);
    return results;
    }




    



// Define any helper functions here
void wordleHelper(
    const std::string& pattern,
    const std::set<std::string>& dict,
    const std::set<std::string>& prefixes,
    std::string& current,
    std::string floating,
    size_t idx,
    const std::vector<int>& blanksRem,
    std::set<std::string>& results)
{
    size_t len = pattern.size();

    if(idx == len) {
        if(floating.empty() && dict.find(current) != dict.end()) {
            results.insert(current);
        }
        return;
    }

    if((int)floating.size() > blanksRem[idx]) {
        return;
    }

    if(pattern[idx] != '-') {
        char c = pattern[idx];
        current[idx] = c;

        std::string nextFloating = floating;
        size_t pos = nextFloating.find(c);
        if(pos != std::string::npos) {
            nextFloating.erase(pos, 1);
        }

        std::string pref = current.substr(0, idx + 1);
        if(prefixes.find(pref) == prefixes.end()) {
            return;
        }

        wordleHelper(pattern, dict, prefixes, current, nextFloating, idx + 1, blanksRem, results);
    }else {
        int blanksLeft = blanksRem[idx];

        if(blanksLeft == (int)floating.size()) {
            size_t k = 0;
            while(k < floating.size()) {      // loop #4
                char c = floating[k];
                current[idx] = c;

                std::string nextFloating = floating;
                nextFloating.erase(k, 1);

                std::string pref = current.substr(0, idx + 1);
                if(prefixes.find(pref) != prefixes.end()) {
                    wordleHelper(pattern, dict, prefixes, current, nextFloating,
                                 idx + 1, blanksRem, results);
                }
                k = k + 1;
            }
        }else {
            char c = 'a';
            while(c <= 'z') {                 // loop #5
                current[idx] = c;

                std::string nextFloating = floating;
                size_t pos = nextFloating.find(c);
                if(pos != std::string::npos) {
                    nextFloating.erase(pos, 1);
                }

                std::string pref = current.substr(0, idx + 1);
                if(prefixes.find(pref) != prefixes.end()) {
                    wordleHelper(pattern, dict, prefixes, current, nextFloating,
                                 idx + 1, blanksRem, results);
                }

                c = (char)(c + 1);
            }
        }
    }
}
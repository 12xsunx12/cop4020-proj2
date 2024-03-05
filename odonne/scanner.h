/*      @Author:            Regan O'Donnell
 *      @Professor:         Dr. Bernd Owsnicki-Klewe
 *      @Course Title:      Programming Languages
 *      @Course Number:     COP4020 202401 */

/*      ~ Code Formatting ~
 *      1. #include
 *      2. class declaration
 *      3. private:
 *      4. public:
 *      5. #endif */

#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>         // input and output
#include <fstream>          // file input and output
#include <unordered_map>    // similar to dictionaries in python; hashmaps
#include <unordered_set>    // O(1) lookup; slap the entire english alphabet and all 1-9 digits into a set
#include <algorithm>
#include <vector>

#include "token.h"

class Scanner {
    private:
        std::unordered_map<char, std::string> opTable;
        std::unordered_map<std::string, std::string> keywordTable;
        std::unordered_set<char> ALPHABET;
        std::unordered_set<int> DIGITS;
        std::string fileName;
        std::ifstream ifs;
        std::string currentLine;
        int totalLines;
        std::vector<Token> tokens;

        bool openFile();
        void nextLine();
        std::string clean(const std::string& input);

        bool scanOp(long unsigned int& currentLocation);
        bool scanKeyword(long unsigned int& currentLocation);
        bool scanIdentifier(long unsigned int& currentLocation);
        bool scanVar(long unsigned int& currentLocation, int depth);
        bool scanNumber(long unsigned int& currentLocation);

        void initAll();             // run all `init` functions
        void initOpTable();         // init. the opTable hashmap
        void initKeywordTable();    // init. the keywordTable hashmap
        void initAlphabet();        // init. the ALPHABET set
        void initDigits();          // init. the DIGITS set

    public:
        std::string VAR = "var";

        Scanner();                              // default
        Scanner(std::string fileName);          // param
        void scan();                            // scan and tokenize the entire file
        void test();                            // used for testing priv. functions in main
        bool openFile(std::string fileName);    // open a `source code` file given a string; sets fileName to input as well
        void printTokens();                     // print all tokens in the vector

        std::string getFileName();
        std::string getCurrentLine();
        std::vector<Token> getTokens();
        void setFileName(std::string a);
        void setCurrentLine(std::string a);
};

#endif

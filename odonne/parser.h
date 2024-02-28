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

#ifndef PARSER_H
#define PARSER_H

#include <iostream>         // input and output
#include <fstream>          // file input and output
#include <unordered_map>    // similar to dictionaries in python; hashmaps
#include <unordered_set>    // O(1) lookup; slap the entire english alphabet and all 1-9 digits into a set
#include <algorithm>
#include <vector>

#include "token.h"

class Parser {
    private:
        std::vector<Token> tokens;
        bool parseParen();          // checks for paren and bracket notation
        bool parseIdentifier();     // checks for unacceptable identifier naming schemes
        bool parseIdentifierWithOperator();
        bool parseVar();            // checks that all identifiers were declared with var
        bool parseVarHelper(long unsigned int a, int b);      // recursive descent function that checks for identifiers after a var token
        bool parseBegin();          // checks for proper expressions

    public:
        Parser(std::vector<Token> tokens);
        void printTokens();
        void parse(); // run all checks
};

#endif

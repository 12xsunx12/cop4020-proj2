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

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <string>

#include "token.h"

class Parser {
    private:
        std::vector<Token> tokens;
        std::string fileName;

        bool parseParen();
        bool parseIdentifier();
        bool parseIdentifierWithOperator();
        bool parseVar();
        bool parseVarHelper(long unsigned int a, int b);
        bool parseBegin();


    public:
        Parser(std::vector<Token> tokens, std::string fileName);
        void printTokens();
        void parse(); // run all checks
};

#endif

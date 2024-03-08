
#include "rpn-generator.h"
#include "token.h"
#include <cctype>
#include <cstddef>

RpnGen::RpnGen(std::vector<Token> a) {
    tokens = a;
}

// initialize the operand hash-table
// @param   null
// @return  void
void RpnGen::initOpTable() {
    opTable['(']    = "lParen";
    opTable[')']    = "rParen";
    opTable['{']    = "lCurly";
    opTable['}']    = "rCurly";
    opTable['+']    = "plusSym";
    opTable['-']    = "minusSym";
    opTable['*']    = "multSym";
    opTable['/']    = "divSym";
    opTable[';']    = "semi";
    opTable[',']    = "comma";
    opTable['=']    = "equalSym";
}

// re-organizes the existing tokens into RPN notation and places them in a new vector called, "postfix"
// @param   ...int& i       ~ passed from a for loop; current location in the tokens vector
// @param   int depth       ~ current depth of the recursive call
// @param   Token oprater   ~ most recently scanned operator (iterating from left -> right in the vector, or, start inside parens if they exist)
// @param   Token lVal      ~ the value we are storing this expression inside of... ['STORE', <this value>]
void RpnGen::generate() {
    Token lVal;
    Token oprater;

    // loop through all tokens
    for (int i = 1; i < tokens.size() - 1; i++) {

        // if token is an equal sign, we found an expression
        if (tokens.at(i).tokenType == "equalSym") {
            lVal = tokens.at(i-1);              // store lVal for later
            postfix.push_back(tokens.at(i+1));  // add next variable to postfix
            i += 2;                             // skip ahead by 2


        }
    }
}

void RpnGen::run() {
    // start at 1 (not 0), because first token is always begin
    initOpTable();
    generate();
}

void RpnGen::printRpn() {
    std::cout << "\n\nPostfix Notation: " << std::endl;
    for (long unsigned int i=0; i<postfix.size(); i++) {
        std::cout << postfix.at(i).lexeme << std::endl;
    }
}

























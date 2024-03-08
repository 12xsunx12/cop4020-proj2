
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
void RpnGen::generate(int i, int depth, Token oprater, Token lVal) {
    // current token is end, stop scanning the vector
    if (tokens.at(i).tokenType == "end") {
        return;

    // current token is a semi
    } else if (tokens.at(i).tokenType == "semi") {
        std::cout << "\n\nGenerate done, stats:" << std::endl << "i: " << i << "\ndepth: " << depth << "\nlastOp: " << oprater.lexeme << "\nlVal: " << lVal.lexeme << std::endl;
        generate(i+1, depth+1, Token(), lVal);    // continue to next token

    // current token is an equal sign
    } else if (tokens.at(i).tokenType == "equalSym") {
        lVal = tokens.at(i - 1);            // set lVal to identifier left of equal sign
        postfix.push_back(tokens.at(i+1));  // add the identifier right after the equal sign
        generate(i+2, depth+1, oprater, lVal);

    // current token is +, -, /, *, etc...
    } else if (opTable.count(tokens.at(i).lexeme[0]) > 0) {
        oprater = tokens.at(i);                 // current token is an operator in the hashtable, store for later in the next iteration
        generate(i+1, depth+1, oprater, lVal);    // continue to next token

    // current token is a variable
    } else if (tokens.at(i).tokenType == "idenSym") {
        postfix.push_back(tokens.at(i));        // add current identifier to postfix
        postfix.push_back(oprater);             // add the most recently scanned operator to postfix
        generate(i+1, depth+1, oprater, lVal);    // continue to next token

    // if it fails, just try again on the next token
    } else {
        generate(i+1, depth+1, Token(), lVal);  // continue to next token
    }
}

void RpnGen::run() {
    // start at 1 (not 0), because first token is always begin
    generate(1, 0, tokens.at(1), Token());
}

void RpnGen::printRpn() {
    std::cout << "\n\nPostfix Notation: " << std::endl;
    for (long unsigned int i=0; i<postfix.size(); i++) {
        std::cout << postfix.at(i).lexeme << std::endl;
    }
}

























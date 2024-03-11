
#include "rpn-generator.h"
#include "token.h"
#include <cctype>
#include <cstddef>
#include <fstream>

RpnGen::RpnGen(std::vector<Token> a, std::string b) {
    tokens = a;
    fileName = b;
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

    //first, create postfix for expressions wrapped in parens
    for (long unsigned int a = 0; a < tokens.size(); a++) {
        if (tokens.at(a).tokenType == "lParen") {
            generateHelper(a+1, 0, Token(), Token());
        }
    }

    // loop through all tokens, after creating postfix for (Paren Expressions)
    for (long unsigned int i = 1; i < tokens.size(); i++) {

        // skip over paren expressions, we have already scanned them
        if (tokens.at(i).lexeme[0] == '(') {
            for (long unsigned int j = i; j < tokens.size() - 1; j++) {
                if (tokens.at(j).lexeme[0] == ')') {
                    i = j;
                    break;
                }
            }
        }

        // if token is an equal sign, we found an expression
        if (tokens.at(i).tokenType == "equalSym") {
            lVal = tokens.at(i-1);              // store lVal for later

            if ((tokens.at(i+1).lexeme[0] != '(') || (tokens.at(i+1).lexeme[0] != ')')) {
                postfix.push_back(tokens.at(i+1));  // add next variable to postfix if (NOT null || paren)
            }

            if (tokens.at(i+1).lexeme[0] != '(') {
                i += 2;                         // skip ahead by 2, only if next character isn't a paren
            } else {
                continue;
            }

            // expression has been found, now iterate over the tokens within that expression
            for (long unsigned int j = i; j < tokens.size(); j++) {

                // write the STORE, break from j loop, and skip past all the scanned tokens (i = j;)
                if (tokens.at(j).tokenType == "semi") {
                    i = j;
                    lVal.tokenType = "lVal";
                    postfix.push_back(lVal);
                    break;
                }

                // if current character is an operator (+,-,/,*, etc... NOT equal or var, however)
                if (((tokens.at(j).lexeme[0] != '=') || (tokens.at(j).tokenType != "var")) && (opTable.count(tokens.at(j).lexeme[0]) > 0)) {

                    if ((tokens.at(j+1).lexeme[0] != '(') || (tokens.at(j+1).lexeme[0] != ')')) {
                        oprater = tokens.at(j);
                    }
                    continue;

                // current token is a variable, add it to post fix, add most recent operater, then continue
                } else if ((tokens.at(j).tokenType == "idenSym") || (tokens.at(j).tokenType == "digit")) {
                    postfix.push_back(tokens.at(j));

                    // make sure oprater is (NOT null || paren) before adding it to postfix
                    if ((opTable.count(oprater.lexeme[0]) > 0) || (oprater.lexeme[0] != '(') || (oprater.lexeme[0] != ')')) {
                        postfix.push_back(oprater);
                    }
                    continue;
                }
            }
        }
    }
}

// find the rParen) belonging to the input lParen(, then generate postfix for that expression
void RpnGen::generateHelper(int lParen, int rParen, Token lVal, Token oprater) {

    // locate the following rParen index in the tokens vector
    // if it finds another lParen, recursive call with the same logic
    for (long unsigned int i = lParen; i < tokens.size(); i++) {
        if (tokens.at(i).tokenType == "lParen") {
            generateHelper(i+1, 0, lVal, Token());
        } else if (tokens.at(i).tokenType == "rParen") {
            rParen = i;
            break;
        }
    }

    // loop through the tokens in-between the paren expression and generate postfix
    for (int i = lParen; i < rParen; i++) {

        // if current character is an operator (+,-,/,*, etc... NOT equal or var, however)
            if (((tokens.at(i).lexeme[0] != '=') || (tokens.at(i).tokenType != "var")) && (opTable.count(tokens.at(i).lexeme[0]) > 0)) {
                oprater = tokens.at(i);
                continue;

            // current token is a variable, add it to post fix, add most recent operater, then continue
            } else if ((tokens.at(i).tokenType == "idenSym") || (tokens.at(i).tokenType == "digit")) {
                postfix.push_back(tokens.at(i));

                // make sure oprater is not null or paren
                if (opTable.count(oprater.lexeme[0]) > 0) {
                    postfix.push_back(oprater);
                }
                continue;
            }
    }

    return;
}

void RpnGen::runRpnGeneration() {
    // start at 1 (not 0), because first token is always begin
    initOpTable();
    generate();
}

void RpnGen::printRpn() {
    std::cout << "~ Postfix Notation ~" << std::endl;
    for (long unsigned int i=0; i<postfix.size(); i++) {
        std::cout << postfix.at(i).lexeme << std::endl;
    }
}

void RpnGen::printRpnToFile() {
    ofs.open("RPN-Notation.txt", std::ios::app);

    if (!ofs.is_open()) {
        std::cout << "Error: could not open file: " << std::endl;
        return;
    }

    ofs << "RPN for file: " << fileName << std::endl;

    for (long unsigned int i=0; i < postfix.size(); i++) {
        if (postfix.at(i).tokenType == "idenSym" || postfix.at(i).tokenType == "digit") {
            ofs << "['RVAL', '" + postfix.at(i).lexeme + "']" << std::endl;
        } else if (postfix.at(i).tokenType == "lVal") {
            ofs << "['STORE', '" + postfix.at(i).lexeme + "']" << std::endl;
        } else if (postfix.at(i).tokenType == "plusSym") {
            ofs << "['ADD']" << std::endl;
        } else if (postfix.at(i).tokenType == "minusSym") {
            ofs << "['MINUS']" << std::endl;
        } else if (postfix.at(i).tokenType == "divSym") {
            ofs << "['DIVIDE']" << std::endl;
        } else if (postfix.at(i).tokenType == "multSym") {
            ofs << "['MULTIPLY']" << std::endl;
        }
    }

    ofs << std::endl;

    ofs.close();
}

























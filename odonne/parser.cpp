/*      @Author:            Regan O'Donnell
 *      @Professor:         Dr. Bernd Owsnicki-Klewe
 *      @Course Title:      Programming Languages
 *      @Course Number:     COP4020 202401 */

/*      ~ Code Formatting ~
 *      1. #include
 *      2. public default constructor
 *      3. public param constructor
 *      4. private functions
 *      5. (private) getters & setters
 *      6. public functions */

#include "parser.h"

// param constructor
// @param   vector<Token> tokens    ~ vector containing all tokens collected by the (scanner.cpp & scanner.h) program
// @return  void
Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
}

// check how many left parens match right parens
// @param   null
// @return  true    ~ left parens  = right parens
// @return  false   ~ left parens != right parens
bool Parser::parseParen() {
    int lParen = 0, rParen = 0;

    // count all parens
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens.at(i).tokenType == "lParen") lParen += 1;
        if (tokens.at(i).tokenType == "rParen") rParen += 1;
    }

    // check to see if they're the same number
    if (lParen != rParen) {
        std::cout << "Error: paren" << std::endl;
        return false;
    }

    return true;
}

// ensure that all identifier-tokens are legal
// @param   null
// @return  true    ~ current identifier-token is legal
// @return  false   ~ current identifier-token is NOT legal
bool Parser::parseIdentifier() {
    // for every token in the vector
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        // if token ends with underscore...
        if (tokens.at(i).lexeme.back() == '_') {
            std::cout << "Error: Identifier ends with underscore on " << "Line: " << tokens.at(i).lineNumber << "\tIdentifier: " << tokens.at(i).lexeme << std::endl;
            return false;
        }

        // if token has two under-scores next to each other in it's name
        for (long unsigned int j = 0; j < tokens.at(i).lexeme.length(); j++) {
            if (tokens.at(i).lexeme[j] == '_' && tokens.at(i).lexeme[j+1] == '_') {
                std::cout << "Error: Identifier has two consecutive underscores " << "Line: " << tokens.at(i).lineNumber << "\tIdentifier: " << tokens.at(i).lexeme << std::endl;
                return false;
            }
        }
    }

    return true;
}

// ensure that once a identifier-token is seen, it is followed by a operator-token
// @param   null
// @return  true    ~ identifier DOES have operator ahead of it
// @return  false   ~ identifier DOES NOT have operator ahead of it
bool Parser::parseIdentifierWithOperator() {
    // Iterate through tokens, skipping the last one
    for (long unsigned int i = 0; i < tokens.size() - 1; i++) {
        // Check if the current token is an identifier
        if (tokens.at(i).tokenType == "idenSym") {
            // Check if the next token is a valid operator
            if (tokens.at(i + 1).tokenType != "equalSym" &&
                tokens.at(i + 1).tokenType != "minusSym" &&
                tokens.at(i + 1).tokenType != "plusSym" &&
                tokens.at(i + 1).tokenType != "multSym" &&
                tokens.at(i + 1).tokenType != "divSym" &&
                tokens.at(i + 1).tokenType != "semi" &&
                tokens.at(i + 1).tokenType != "rParen" &&
                tokens.at(i + 1).tokenType != "comma"){
                std::cout << "Error: Identifier not followed by a valid operator " << "Line: " << tokens.at(i).lineNumber << "\tIdentifier: " << tokens.at(i).lexeme << std::endl;
                return false;
            }
        }
    }
    return true;
}

// scan every identifier, if (not declared | var token not behind it) = then there is an error
// @param   null
// @return  true    ~ all identifiers were declared with var
// @return  false   ~ NOT all identifiers were declared with var
bool Parser::parseVar() {
    bool success = true;

    // iterate through all tokens and declare them
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens.at(i).tokenType == "varSym") {
            return parseVarHelper(i, 0);
        }
    }

    // iterate through all tokens (past i), and set the same identifiers as declared if they exist else-where
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens.at(i).tokenType == "idenSym") {
            if (tokens.at(i).declared == true) {
                for (int j = i; j < tokens.size(); j++) {
                    if (tokens.at(j).lexeme == tokens.at(i).lexeme){
                        tokens.at(j).declared = true;
                    }
                }
            }
        }
    }

    // finally, iterate through the list again and flag all identifiers that are not declared
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens.at(i).tokenType == "idenSym") {
            if (tokens.at(i).declared == false) {
                std::cout << "Error: Identifier, \"" << tokens.at(i).lexeme << "\" was not declared with \"var\" on line: " << tokens.at(i).lineNumber << std::endl;
                success = false;
            }
        }
    }

    if (success){
        return true;
    } else {
        return false;
    }
}

bool Parser::parseVarHelper(long unsigned int currentI, int depth) {
    // if next line is an identifier or a comma, it is declared
        // -> continue to next identifier, or eol
    currentI += 1;
    if (currentI > tokens.size() || tokens.at(currentI).tokenType == "semi") {
        return true;
    } else if (tokens.at(currentI).tokenType == "idenSym" || tokens.at(currentI).tokenType == "comma") {
        tokens.at(currentI).declared = true;
        return parseVarHelper(currentI, ++depth);
    } else {
        return false;
    }
}

// first token in vector must be the begin-token (and spelled right)
// @param   null
// @return  true    ~ first token (IS begin | spelled right)
// @return  false   ~ first token (IS NOT begin | spelled wrong)
bool Parser::parseBegin() {
    // if the first token isn't 'begin', then flag it
    if (tokens.at(0).tokenType != "beginSym") {
        std::cout << "Error: \"begin\" either mispelled, not the first line, or doesn't exist. Please check at line: " << tokens.at(0).lineNumber << "\t Lexeme: " << tokens.at(0).lexeme << std::endl;
        return false;
    }
    return true;
}

// run all 'parse*' functions
// @param   null
// @return  void
void Parser::parse() {
    bool badCode = true;
    if (!parseParen())                      badCode = false;
    if (!parseIdentifier())                 badCode = false;
    if (!parseIdentifierWithOperator())     badCode = false;
    if (!parseVar())                        badCode = false;
    if (!parseBegin())                      badCode = false;

    if (badCode) {
        std::cout << "success! No errors found by parser." << std::endl;
    }
}

// iteratire through 'tokens' and print information to terminal
// @param   null
// @return  void
void Parser::printTokens() {
    std::cout << "Size of Vector: " << tokens.size() << std::endl;
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        std::cout << "Token: \t" << tokens.at(i).tokenType << "    \tLexeme: \t" << tokens.at(i).lexeme << "\tLine Number:\t" << tokens.at(i).lineNumber << std::endl;
    }
}

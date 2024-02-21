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

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
}

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
                tokens.at(i + 1).tokenType != "rParen") {
                std::cout << "Error: Identifier not followed by a valid operator " << "Line: " << tokens.at(i).lineNumber << "\tIdentifier: " << tokens.at(i).lexeme << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool Parser::parseBegin() {
    // if the first token isn't 'begin', then flag it
    if (tokens.at(0).tokenType != "beginSym") {
        std::cout << "Error: \"begin\" either mispelled, not the first line, or doesn't exist. Please check at line: " << tokens.at(0).lineNumber << "\t Lexeme: " << tokens.at(0).lexeme << std::endl;
        return false;
    }
    return true;
}

void Parser::parse() {
    if (parseParen() && parseIdentifier() && parseIdentifierWithOperator() && parseBegin()) std::cout << "success! No errors found by parser." << std::endl;
}

void Parser::printTokens() {
    std::cout << "Size of Vector: " << tokens.size() << std::endl;
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        std::cout << "Token: \t" << tokens.at(i).tokenType << "    \tLexeme: \t" << tokens.at(i).lexeme << "\tLine Number:\t" << tokens.at(i).lineNumber << std::endl;
    }
}

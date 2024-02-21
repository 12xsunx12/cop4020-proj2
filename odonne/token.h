#ifndef TOKEN_H
#define TOKEN_H

struct Token {
    std::string lexeme;
    std::string tokenType;
    int lineNumber;
};

#endif

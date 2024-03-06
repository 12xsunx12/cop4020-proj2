#ifndef RPN_GEN
#define RPN_GEN

#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
#include <map>

#include "token.h"

class RpnGen {
    private:
        std::vector<Token> tokens;
        std::vector<Token> postfix;
        std::ifstream ifs;
        std::ofstream ofs;
        std::stack<Token> stak;
        int precedence(Token oprater);
        void generate(long unsigned int& i, int depth, Token oprater, Token lVal);

    public:
        RpnGen(std::vector<Token> tokens);
        void printRpn();
};

#endif

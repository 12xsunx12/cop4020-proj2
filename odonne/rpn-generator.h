#ifndef RPN_GEN
#define RPN_GEN

#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <unordered_map>

#include "token.h"

class RpnGen {
    private:
        std::vector<Token> tokens;
        std::vector<Token> postfix;
        std::unordered_map<char, std::string> opTable;
        std::ifstream ifs;
        std::ofstream ofs;
        std::stack<Token> stak;
        void initOpTable();
        void generate(int i, int depth, Token oprater, Token lVal);

    public:
        RpnGen(std::vector<Token> tokens);
        void run();
        void printRpn();
        void printRpnToFile();
};

#endif

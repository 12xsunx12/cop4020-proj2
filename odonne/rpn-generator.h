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
        std::string fileName;
        std::unordered_map<char, std::string> opTable;
        std::ifstream ifs;
        std::ofstream ofs;
        std::stack<Token> stak;

        void initOpTable();
        void generate();
        void generateHelper(int lParen, int rParen, Token lVal, Token oprater);


    public:
        RpnGen(std::vector<Token> tokens, std::string fileName);
        void runRpnGeneration();
        void printRpn();
        void printRpnToFile();
};

#endif


#include "rpn-generator.h"
#include <cctype>

RpnGen::RpnGen(std::vector<Token> a) {
    tokens = a;
}

// Function to determine the precedence of operators
int RpnGen::precedence(Token op) {
    if (op.tokenType == "plusSym" || op.tokenType == "minusSym")
        return 1;
    if (op.tokenType == "multSym" || op.tokenType == "divSym")
        return 2;
    return 0;  // Assuming all other characters have lower precedence (including parentheses)
}


std::vector<Token> RpnGen::generate() {
    // store operands and operators into this vector in "postfix" notation
    std::vector<Token> postfix;

    for (long unsigned int i = 0; i < tokens.size(); i++) {
        // current token is a operand
        if (tokens.at(i).tokenType == "idenSym" || tokens.at(i).tokenType == "digit") postfix.push_back(tokens.at(i));
        // current token is left parentheses
        else if (tokens.at(i).tokenType == "lParen") stak.push(tokens.at(i));
        // current token is right parentheses
        else if (tokens.at(i).tokenType == "rParen") {
            while (!stak.empty() && stak.top().tokenType != "lParen") {
                postfix.push_back(stak.top());
                stak.pop();
            }
            // remove lparen from stack after loop
            stak.pop();
        // operator encountered (+, -, *, /)
        } else {
            while (!stak.empty() && (precedence(stak.top()) >= precedence(tokens.at(i)))) {
                postfix.push_back(stak.top());
                stak.pop();
            }
            // push current operator onto stack after loop
            stak.push(tokens.at(i));
        }

        // add remaining operators from stack -> postfix
        while (!stak.empty()){
            postfix.push_back(stak.top());
            stak.pop();
        }
    }

    return postfix;
}

void RpnGen::printRpn() {
    std::vector<Token> postfix = generate();
    std::cout << "postfix: ";
    for (long unsigned int i = 0; i < postfix.size(); i++) {
        std::cout << postfix.at(i).lexeme << " ";
    }
}

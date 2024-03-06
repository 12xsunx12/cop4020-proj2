
#include "rpn-generator.h"
#include "token.h"
#include <cctype>

RpnGen::RpnGen(std::vector<Token> a) {
    tokens = a;
}

// re-organizes the existing tokens into RPN notation and places them a new vector called, "postfix"
// @param   ...int& i       ~ passed from a for loop; current location in the tokens vector
// @param   int depth       ~ current depth of the recursive call
// @param   Token oprater   ~ most recently scanned operator (iterating from left -> right in the vector, or, start inside parens if they exist)
// @param   Token lVal      ~ the value we are storing this expression inside of... ['STORE', <this value>]
void RpnGen::generate(long unsigned int& i, int depth, Token oprater, Token lVal) {

    // x = a + b
    // ['RVAL', a]
    // ['RVAL', b]
    // ['ADD']
    // ['STORE', x]

    // recursive descent algorithem for generating Rpn

    // if equal sign detected, begin algorithem (happens outside of this function)
        // if i = semicolon -> return;
        // else...
            //
}

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

#include "scanner.h"

// default constructor
// @param null
// @return void
Scanner::Scanner() {
    this->fileName = "";
    totalLines = 0;
    initAll();
}

// param constructor
// @param string fileName ~ name of the source code file that will be scanned & parsed
// @return void
Scanner::Scanner(std::string fileName) {
    this->fileName = fileName;
    totalLines = 0;
    initAll();
}

// initializes the 'inputFileStream ifs' with the 'fileName' variable
// @param null
// @return true   ~ the file WAS successfully opened
// @return false  ~ the file WAS NOT successfully opened
bool Scanner::openFile() {
    ifs.open(fileName);
    if (!ifs.is_open()) {
        std::cout << "Error: file doesn't exist or couldn't be opened" << std::endl;
        return false;
    }
    return true;
}

// initializes the 'inputFileStream ifs' with the 'fileName' variable
// @param  string fileName  ~ name of the source-code file
// @return true             ~ the file WAS successfully opened
// @return false            ~ the file WAS NOT successfully opened
bool Scanner::openFile(std::string fileName) {
    ifs.open(fileName);
    if (!ifs.is_open()) {
        std::cout << "Error: file doesn't exist or couldn't be opened" << std::endl;
        return false;
    }
    return true;
}

// grabs and sets the next line in the given source-code-file, given source-code file is 'fileName'
// @param null
// @return void
void Scanner::nextLine() {
    std::string temp;
    std::getline(ifs, temp);
    setCurrentLine(temp);
    totalLines++;
}

// removes all white-spaces and new-lines from a string input, returns the 'cleaned' string (also uses pass-by-reference)
// @param const string& input  ~ string to be cleaned of white-spaces & new-lines
// @return result              ~ original string, but now with no white-spaces & new-lines
std::string Scanner::clean(const std::string& input) {
    std::string result = input;

    // Remove whitespaces
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());

    // Remove newline characters
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

    return result;
}

// scan current character 'currentLocation' and return (true | false) if (that input can be found inside the 'opTable' hash-map)
// @param  ...int& currentLocation   ~ the (iterator | i) value from a for loop, a for loop iterating through a string
// @return true                      ~ the characer located at (currentLocation | i | iterator) CAN be found inside the 'opTable'
// @return false                     ~ the characer located at (currentLocation | i | iterator) CAN NOT be found inside the 'opTable'
bool Scanner::scanOp(long unsigned int& currentLocation) {
    char tempChar = currentLine[currentLocation];

    // opTable.count ~ return a count for everytime the input was found in the table
    if (opTable.count(tempChar) > 0) {
        Token temp;
        temp.tokenType = opTable[tempChar];
        temp.lexeme = tempChar;
        temp.lineNumber = totalLines;
        tokens.push_back(temp);
        return true;
    } else {
        return false;
    }
}

// scan current character 'currentLocation' and return (true | false) if (that input can be found inside the 'keywordTable' hash-map)
// @param  ...int& currentLocation   ~ the (iterator | i) value from a for loop, a for loop iterating through a string
// @return true                      ~ the created sub-string WAS FOUND inside the 'keywordTable'
// @return false                     ~ the created sub-string WAS NOT FOUND inside the 'keywordTable'
bool Scanner::scanKeyword(long unsigned int& currentLocation) {
    // all keywords in table are less than or equal to 6 characters
    for (int len = 1; len <= 6; len++) {
        // check if there are enough characters remaining to form a keyword of length 'len'
        if (currentLocation + len <= currentLine.length()) {
            // build sub-string (from currentLine) of the next 'len' characters
            std::string tempStr = currentLine.substr(currentLocation, len);
            // opTable.count ~ return a count for everytime the input was found in the table
            if (keywordTable.count(tempStr) > 0) {
                // create token
                Token temp;
                temp.tokenType = keywordTable[tempStr];
                temp.lexeme = tempStr;
                temp.lineNumber = totalLines;
                tokens.push_back(temp);
                // move the current location to the end of the matched keyword
                currentLocation += len - 1;
                return true;
            }
        }
    }
    return false; // no keyword found
}

// inside main, if all scan-related-functions return false, we can assume that our 'currentLocation' involves an identifier
// @param   ...int& currentLocation  ~ the (iterator | i) value from a for loop, a for loop iterating through a string
// @return  true                     ~ the created 'tempStr' is an identifier
// @return  false                    ~ the created 'tempStr' is NOT an identifier
bool Scanner::scanIdentifier(long unsigned int& currentLocation) {
    std::string tempStr;
    int idenLength = 0;

    // Continue scanning characters as long as they are alphanumeric
    while (isalnum(currentLine[currentLocation + idenLength]) || currentLine[currentLocation + idenLength] == '_') {
        tempStr += currentLine[currentLocation + idenLength];
        idenLength++;

        // Check if the next character is an operator or if we have reached the end of the line
        if (!isalnum(currentLine[currentLocation + idenLength]) && (currentLine[currentLocation + idenLength]) != '_') {
            // create token for the identifier
            Token temp;
            temp.tokenType = "idenSym";
            temp.lexeme = tempStr;
            temp.lineNumber = totalLines;
            tokens.push_back(temp);
            currentLocation += idenLength - 1;
            return true;
        }
    }

    // No identifier found
    return false;
}

// if a digit is encountered at the 'currentLocation' of our string, it is assumed to be a digit
// @param   ...int& currentLocation     ~ the (iterator | i) value from a for loop, a for loop iterating through a string
// @return  true                        ~ the char at 'currentLocation' was a digit
// @return  false                       ~ the char at 'currentLocation' was NOT part of a digit
bool Scanner::scanNumber(long unsigned int& currentLocation) {
    std::string tempStr;
    long unsigned int numStart = currentLocation; // Store the start index of the number

    // Scan until a non-digit character is encountered
    while (isdigit(currentLine[currentLocation])) {
        currentLocation++;
    }

    // Check if any digits were found
    if (currentLocation > numStart) {
        // Create a substring with the digits
        tempStr = currentLine.substr(numStart, currentLocation - numStart);

        // Create a token
        Token temp;
        temp.tokenType = "digit"; // Assuming this is the token type for numerical digits
        temp.lexeme = tempStr;
        temp.lineNumber = totalLines; // figure out how to set line number later
        tokens.push_back(temp);

        // Decrement currentLocation to point to the character after the number
        currentLocation--;

        return true;
    } else {
        return false; // No digits found
    }
}

// run all 'init*' functions
// @param   null
// @return  void
void Scanner::initAll() {
    initOpTable();
    initKeywordTable();
    initAlphabet();
    initDigits();
}

// initialize the operand hash-table
// @param   null
// @return  void
void Scanner::initOpTable() {
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

// initialize the keyword hash-table
// @param   null
// @return  void
void Scanner::initKeywordTable() {
    keywordTable["while"]    = "whileSym";
    keywordTable["return"]   = "returnSym";
    keywordTable["if"]       = "ifSym";
    keywordTable["else"]     = "elseSym";
    keywordTable["do"]       = "doSym";
    keywordTable["int"]      = "intSym";
    keywordTable["string"]   = "stringSym";
    keywordTable["begin"]    = "beginSym";
    keywordTable["end."]     = "endSym";
}

// initialize the ALPHABET set
// @param   null
// @return  void
void Scanner::initAlphabet() {
    // all lower case letters
    for (int i = 'a'; i < 'z'; i++){
        ALPHABET.insert(i);
    }

    // all uppercase letters
    for (int i = 'A'; i < 'Z'; i++){
        ALPHABET.insert(i);
    }
}

// initialize the DIGIT set
// @param   null
// @return  void
void Scanner::initDigits() {
    // all digits 0-9
    for (int i = 0; i < 10; i++){
        DIGITS.insert(i);
    }
}

// return 'fileName'
// @param   null
// @return  string fileName
std::string Scanner::getFileName() {
    return this->fileName;
}

// return 'currentLine'
// @param   null
// @return  string currentLine
std::string Scanner::getCurrentLine() {
    return this->currentLine;
}

// return a vector containing all tokens
// @param   null
// @return  vector<Token> tokens
std::vector<Token> Scanner::getTokens() {
    return this->tokens;
}

// set the 'fileName'
// @param   string a
// @return  void
void Scanner::setFileName(std::string a){
    this->fileName = a;
}

// set the 'currentLine'
// @param   string a
// @return  void
void Scanner::setCurrentLine(std::string a) {
    this->currentLine = a;
}

// open source-code file, scan over every char, create tokens using 'scan*' functions
// @param   null
// @return  void
void Scanner::scan() {
    // open the file
    openFile(fileName);
    // iterate through every line in the file
    while (std::getline(ifs, currentLine)) {
        totalLines += 1;
        // remove all whitespaces and newlines from the string
        currentLine = clean(currentLine);

        // begin iterating over every character in the string and feeding it into subsequent, more logical, scanner functions, that check for edge-cases
        for (long unsigned int i = 0; i < currentLine.length(); i++) {
            // skips over any comments
            if (currentLine.find('~') != std::string::npos) {
                break;
            }

            // the actual 'scanning' part
            if (scanKeyword(i)) {
                continue;
            } else if (scanOp(i)) {
                continue;
            } else if (scanNumber(i)) {
                continue;
            } else if (isalpha(currentLine[i])) {
                scanIdentifier(i);
            }
        }
    }
}

// function the programmer uses for debugging
// @param   null
// @return  void
void Scanner::test() {
    scan();
    printTokens();
}

// iterate through 'tokens' and print all token info to terminal
// @param   null
// @return  void
void Scanner::printTokens() {
    std::cout << "Size of Vector: " << tokens.size() << std::endl;
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        std::cout << "Token: \t" << tokens.at(i).tokenType << "    \tLexeme: \t" << tokens.at(i).lexeme << "\tLine Number:\t" << tokens.at(i).lineNumber << std::endl;
    }
}

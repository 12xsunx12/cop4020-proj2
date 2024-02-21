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
Scanner::Scanner() {
    this->fileName = "";
    totalLines = 0;
    initAll();
}

// param constructor
Scanner::Scanner(std::string fileName) {
    this->fileName = fileName;
    totalLines = 0;
    initAll();
}

bool Scanner::openFile() {
    ifs.open(fileName);
    if (!ifs.is_open()) {
        std::cout << "Error: file doesn't exist or couldn't be opened" << std::endl;
        return false;
    }
    return true;
}

void Scanner::nextLine() {
    std::string temp;
    std::getline(ifs, temp);
    setCurrentLine(temp);
    totalLines++;
}

std::string Scanner::clean(const std::string& input) {
    std::string result = input;

    // Remove whitespaces
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());

    // Remove newline characters
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

    return result;
}

bool Scanner::scanOp(long unsigned int& currentLocation) {
    char tempChar = currentLine[currentLocation];

    // check if the current location is any of the operators
    if (opTable.count(tempChar) > 0) {
        // Create token
        Token temp;
        temp.tokenType = opTable[tempChar];
        temp.lexeme = tempChar;
        temp.lineNumber = totalLines; // figure this out later @regan
        tokens.push_back(temp);
        return true;
    } else {
        return false;
    }
}

bool Scanner::scanKeyword(long unsigned int& currentLocation) {
    // Scan for keywords starting from the current location
    for (int len = 1; len <= 6; len++) {
        // Check if there are enough characters remaining to form a keyword of length 'len'
        if (currentLocation + len <= currentLine.length()) {
            std::string tempStr = currentLine.substr(currentLocation, len);
            if (keywordTable.count(tempStr) > 0) {
                // Create token for the keyword
                Token temp;
                temp.tokenType = keywordTable[tempStr];
                temp.lexeme = tempStr;
                temp.lineNumber = totalLines;
                tokens.push_back(temp);
                // Move the current location to the end of the matched keyword
                currentLocation += len - 1;
                return true;
            }
        }
    }
    return false; // No keyword found
}

bool Scanner::scanIdentifier(long unsigned int& currentLocation) {
    std::string tempStr;
    int idenLength = 0;

    // Continue scanning characters as long as they are alphanumeric
    while (isalnum(currentLine[currentLocation + idenLength]) || currentLine[currentLocation + idenLength] == '_') {
        tempStr += currentLine[currentLocation + idenLength];
        idenLength++;

        // Check if the next character is an operator or if we have reached the end of the line
        if (!isalnum(currentLine[currentLocation + idenLength]) && (currentLine[currentLocation + idenLength]) != '_') {
            // Create token for the identifier
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

void Scanner::initAll() {
    initOpTable();
    initKeywordTable();
    initAlphabet();
    initDigits();
}

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

void Scanner::initDigits() {
    // all digits 0-9
    for (int i = 0; i < 10; i++){
        DIGITS.insert(i);
    }
}

std::string Scanner::getFileName() {
    return this->fileName;
}

std::string Scanner::getCurrentLine() {
    return this->currentLine;
}

std::vector<Token> Scanner::getTokens() {
    return this->tokens;
}

void Scanner::setFileName(std::string a){
    this->fileName = a;
}

void Scanner::setCurrentLine(std::string a) {
    this->currentLine = a;
}

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
            if (currentLine.find('~') != std::string::npos) {
                break;
            }

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

void Scanner::test() {
    scan();
    printTokens();
}

bool Scanner::openFile(std::string fileName) {
    ifs.open(fileName);
    if (!ifs.is_open()) {
        std::cout << "Error: file doesn't exist or couldn't be opened" << std::endl;
        return false;
    }
    return true;
}

void Scanner::printTokens() {
    std::cout << "Size of Vector: " << tokens.size() << std::endl;
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        std::cout << "Token: \t" << tokens.at(i).tokenType << "    \tLexeme: \t" << tokens.at(i).lexeme << "\tLine Number:\t" << tokens.at(i).lineNumber << std::endl;
    }
}

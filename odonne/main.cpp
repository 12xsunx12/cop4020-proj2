/*      @Author:            Regan O'Donnell
 *      @Professor:         Dr. Bernd Owsnicki-Klewe
 *      @Course Title:      Programming Languages
 *      @Course Number:     COP4020 202401 */

/*      ~ Code Formatting ~
 *      1. #include
 *      2. int main() {} */

#include "rpn-generator.h"
#include "scanner.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (std::string(argv[1]) == "all") {
        for (int i = 1; i <= 8; ++i) {
            std::string fileName = "source-code-inputs/a" + std::to_string(i);
            std::cout << fileName << std::endl;
            Scanner s(fileName);
            s.scan();
            Parser p(s.getTokens(), fileName);
            p.parse();
        }
    } else {
        for (int i = 1; i < argc; ++i) {
            Scanner s(argv[i]);
            s.scan();
            Parser p(s.getTokens(), argv[i]);
            p.parse();
        }
    }

    return 0;
}

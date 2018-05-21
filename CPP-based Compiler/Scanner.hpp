//
//  Scanner.hpp
//  CPP-based Compiler
//
//  Created by Ryan Romanosky on 5/13/18.
//  Copyright © 2018 Ryan Romanosky. All rights reserved.
//

#ifndef Scanner_hpp
#define Scanner_hpp

#include <stdio.h>
#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

class Scanner {
private:
    string token; // used to keep track of read char from file
    string listbuffer; // used to keep track of all input from file
    int linenumber;
    string* lexErrorArr;
    string* syntaxErrorArr;
public:
    Scanner();
    string GetToken(ifstream& inp, ofstream& listp);
    bool CheckForReservedChar(const char c);
    bool CheckCharForAlpha(const char c);
    bool CheckCharForDigit(const char c);
    bool CheckCharForWhitespace(const char c);
    string* getLexErrors(void) const;
};



#endif /* Scanner_hpp */

//
//  Parser.hpp
//  CPP-based Compiler
//
//  Created by Ryan Romanosky on 5/16/18.
//  Copyright © 2018 Ryan Romanosky. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctype.h>

using namespace std;


class Parser {
private:
    string currToken;
    string nextToken;
    string validKeywords[35]; // arrat used to keep track of known keywords (reserved words?) such as ")" and "BEGIN"
    enum validKeywordsEnums {
        BEGIN = 0,
        END = 1,
        READ = 2,
        WRITE = 3,
        IF = 4,
        THEN = 5,
        ELSE = 6,
        ENDIF = 7,
        WHILE = 8,
        ID = 9,
        INTLITERAL = 10,
        LPAREN = 11,
        RPAREN = 12,
        SEMICOLON = 13,
        COMMA = 14,
        ASSIGNOP = 15,
        PLUSOP = 16,
        MINUSOP = 17,
        MULTOP = 18,
        DIVOP = 19,
        LESSOP = 20,
        LESSEQOP = 21,
        GREQOP = 22,
        GREATEROP = 23,
        EQUALOP = 24,
        NOTEQOP = 25,
        EQUAL = 26,
        GOTO = 27,
        SCANEOF = 28,
        ERROR= 29,
        COMMENT= 30,
        NOT= 31,
        ANDOP = 32,
        OROP = 33,
        ENDWHILE = 34
    };
public:
    Parser();
    int StartParser(ifstream &inp, ofstream &outp, ofstream &listp);
    int GetTokenType(const string); // main token check, calls three function below
    int CheckForKeyword(const string); // checks passed token for keyword
    int CheckForVarOrDigit(const string);
    
};

#endif /* Parser_hpp */

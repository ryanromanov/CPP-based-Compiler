//
//  Parser.cpp
//  CPP-based Compiler
//
//  Created by Ryan Romanosky on 5/16/18.
//  Copyright © 2018 Ryan Romanosky. All rights reserved.
//

#include "Parser.hpp"
#include "Scanner.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctype.h>
#define keywordArraySize 35

using namespace std;

Parser::Parser() {
    currToken = "";
    nextToken = "";
    sc;
    validKeywords[0] = "BEGIN";
    validKeywords[1] = "END";
    validKeywords[2] = "READ";
    validKeywords[3] = "WRITE";
    validKeywords[4] = "IF";
    validKeywords[5] = "THEN";
    validKeywords[6] = "ELSE";
    validKeywords[7] = "ENDIF";
    validKeywords[8] = "WHILE";
    validKeywords[9] = "ID";
    validKeywords[10] = "INTLITERAL";
    validKeywords[11] = "LPAREN";
    validKeywords[12] = "RPAREN";
    validKeywords[13] = "SEMICOLON";
    validKeywords[14] = "COMMA";
    validKeywords[15] = "ASSIGNOP";
    validKeywords[16] = "PLUSOP";
    validKeywords[17] = "MINUSOP";
    validKeywords[18] = "MULTOP";
    validKeywords[19] = "DIVOP";
    validKeywords[20] = "LESSOP";
    validKeywords[21] = "LESSEQOP";
    validKeywords[22] = "GREQOP";
    validKeywords[23] = "GREATEROP";
    validKeywords[24] = "EQUALOP";
    validKeywords[25] = "NOTEQOP";
    validKeywords[26] = "EQUAL";
    validKeywords[27] = "GOTO";
    validKeywords[28] = "SCANEOF";
    validKeywords[29] = "ERROR";
    validKeywords[30] = "COMMENT";
    validKeywords[31] = "NOT";
    validKeywords[32] = "ANDOP";
    validKeywords[33] = "OROP";
    validKeywords[34] = "ENDWHILE";
}


int Parser::StartParser(ifstream &inp, ofstream &outp, ofstream &listp) {
    bool eofReached = false;
    while (!eofReached) {
        currToken = sc.GetToken(inp, listp);
        nextToken = sc.GetToken(inp, listp);
        cout << "Found token:\t" << currToken << endl;
        if (currToken == "" || nextToken == "") {
            eofReached = true;
        }
    }
    
    return 0;
}




/************************************************************
 This function is the tokentype superfunction that calls two other functions
 - first it calls CheckForKeyword
    - this checks to see if the passed token is a reserved keyword
    - if that doesn't find anything, it calls CheckForVarOrDigit
 - returns false if there isn't
 ************************************************************/
int Parser::GetTokenType(const string token) {
    int tokentype = -1;
    tokentype = CheckForKeyword(token);
    if (tokentype == -1) {
        tokentype = CheckForVarOrDigit(token);
    }
    cout << "Token type found as \"" << validKeywords[tokentype] << "\"\n";
    return tokentype;
}
/************************************************************
 This function will check to see if passed token is a reserved word
 - loops through validKeyword[] and compares token to strings
    - if it finds one, return the index
    - otherwise, return -1
 ************************************************************/
int Parser::CheckForKeyword(const string token) {
    int keyword = -1;
    int i;
    for (i = 0; i < keywordArraySize; i++) {
        if (token == validKeywords[i]) {
            keyword = i;
            break;
        }
    }
    return keyword;
}
/************************************************************
 This function will check to see if token is ID or integer
 - will check token to see if it contains only alphas, only digits, or both
    - only alphas or mixed means token is variable
    - only digits mean it's an intliteral
 ************************************************************/
int Parser::CheckForVarOrDigit(const string token) {
    int varorDigit = -1;
    int length = token.length();
    bool alpha = false;
    bool digit = false;
    int i = 0;
    while (i < length) {
        if (isalpha(token[i])) {
            alpha = true;
        }
        if (isdigit(token[i])) {
            digit = true;
        }
        i++;
    }
    if ((alpha) || (alpha && digit)) {
        varorDigit = ID;
    }
    else if (!alpha && digit) {
        varorDigit = INTLITERAL;
    }
    return varorDigit;
}



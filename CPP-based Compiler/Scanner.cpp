//
//  Scanner.cpp
//  CPP-based Compiler
//
//  Created by Ryan Romanosky on 5/13/18.
//  Copyright © 2018 Ryan Romanosky. All rights reserved.
//

#include "Scanner.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctype.h>


using namespace std;

Scanner::Scanner() {
    token = ""; // prep buffer to null
    listbuffer = "";
    linenumber = 0;
    lexArrSize = 10;
    lexArrIndex = 0;
    syntaxErrorArr = new string[10];
    lexErrorArr = new char [lexArrSize];
}
/************************************************************
 
************************************************************/

string Scanner::GetToken(ifstream &inp, ofstream &listp) {
    char c = '\0'; // read ifstream into this
    bool keepReading = true;
    streampos spos;
    token = "";
    while (keepReading) {
        // save the position in spos ("stream position")
        spos = inp.tellg();
        inp.get(c);
        // check if file pointer has read into the EOF
        // if so, keepReading is set to false and loop will exit
        // Note however that the read char c will still be processed
        if (inp.eof()) {
            keepReading = false;
        }
        else if (CheckForReservedChar(c)) {
            // we need to check to see if it's the only char that has been read
            if (token != "") {
                inp.seekg(spos);
                keepReading = false;
            }
            else {
                token += c;
                listbuffer += c;
                // if it's a reserved, we need to see if it's one of the special ones
                // e.g., :=, <>, >=, etc.
                if (c == ':') {
                    spos = inp.tellg();
                    inp.get(c);
                    if (c == '=') {
                        // we have :=, add and leave
                        token += c;
                        listbuffer += c;
                        keepReading = false;
                    }
                    else {
                        inp.seekg(spos);
                    }
                }
                else if (c == '>') {
                    spos = inp.tellg();
                    inp.get(c);
                    if (c == '=') {
                        token += c;
                        listbuffer += c;
                        keepReading = false;
                    }
                    else {
                        inp.seekg(spos);
                    }
                }
                else if (c == '<') {
                    spos = inp.tellg();
                    inp.get(c);
                    if (c == '=') {
                        token += c;
                        listbuffer += c;
                        keepReading = false;
                    }
                    else if (c == '>') {
                        token += c;
                        listbuffer += c;
                        keepReading = false;
                    }
                    else {
                        inp.seekg(spos);
                    }
                }
                else {
                    keepReading = false;
                }
            }
        }
        else if (CheckCharForAlpha(c)) {
            token += c;
            listbuffer += c;
            //keepReading = true;
        }
        else if(CheckCharForDigit(c)) {
            token += c;
            listbuffer += c;
            //keepReading = true;
        }
        else if (CheckCharForWhitespace(c)) {
            // if this is the first char in token, we will keep reading until we hit a non-whitespace char
            if (token == "") {
                while (CheckCharForWhitespace(c)) {
                    if (c == '\n') {
                        // print listing buffer
                    }
                    else {
                        // add it to the listing buffer
                        listbuffer += c;
                    }
                    spos = inp.tellg();
                    inp.get(c);
                }
                inp.seekg(spos);
            }
            // else we already have a token done and this is the trailing whitespace
            // so reset pointer and exit
            else {
                inp.seekg(spos);
                keepReading = false;
            }
        }
        /************************************************************
         if we reach this else if, we have a lexical error
         - add it to the listing buffer
         -  we should return it as a listing error code
        ************************************************************/
        else {
            if (token == "") {
                listbuffer += c;
                // add to listing error buffer
                // but continue reading for next token
                AddToLexArr(c);
            }
            else {
                // we already have something in the token buffer
                // so add it to listing and set keepreading to false
                listbuffer += c;
                // add to listing error buffer here
                keepReading = false;
            }
            
        }
    }
    return token;
}

/************************************************************
 This function will check to see if the read char from input is a reserved char
 - if so it returns true
 - else it returns false
 ************************************************************/
bool Scanner::CheckForReservedChar(const char c) {
    bool isReserved = false;
    if (c == '(' || c == ')' || c == ';' || c == ':' || c == ',' || c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<') {
        isReserved = true;
    }
    else {
        isReserved = false;
    }
    return isReserved;
}
/************************************************************
 This function will check to see if the read char from input is an alpha
 - if so it returns true
    - else it returns false
************************************************************/
bool Scanner::CheckCharForAlpha(const char c) {
    bool isAlpha = false;
    if (isalpha(c)) {
        isAlpha = true;
    }
    else {
        isAlpha = false;
    }
    return isAlpha;
}
/************************************************************
 This function will check to see if the read char from input is a digit
 - if so it returns true
 - else it returns false
 ************************************************************/
bool Scanner::CheckCharForDigit(const char c) {
    bool isDigit = false;
    if (isdigit(c)) {
        isDigit = true;
    }
    else {
        isDigit = false;
    }
    return isDigit;
}
/************************************************************
 This function will check for whitespace characters
 - returns true if there is one
  - returns false if there isn't
************************************************************/
bool Scanner::CheckCharForWhitespace(const char c) {
    bool iswhitespace = false;
    if(isspace(c)) {
        iswhitespace = true;
    }
    else {
        iswhitespace = false;
    }
    return iswhitespace;
}
/************************************************************
 This function adds a found character to the lex array
 - first it checks to see if we have reached the max_size of the lexArray
    - if so, it calls ResizeLexArray, which will does exactly that
 - then it adds the read character into the lexErrorArray
 - finally it increments the lex array index
************************************************************/
void Scanner::AddToLexArr(const char c) {
    if (lexArrIndex >= lexArrSize) {
        ResizeLexArray();
    }
    lexErrorArr[lexArrIndex] = c;
    lexArrIndex++;
}
/************************************************************
 This function resizes the lexErrorArray (used to hold lexical errors)
 - it copies over the values to a new string* array that holds 5 more elements
 - then it increases the lexArrSize by 5
 - finally it sets the lexErrorArr to the resized array and deletes the resized array
************************************************************/
void Scanner::ResizeLexArray(void) {
    char* resizedArray = new char[lexArrSize + 5];
    int i;
    for (i = 0; i < lexArrSize; i++) {
        resizedArray[i] = lexErrorArr[i];
    }
    lexArrSize+= 5;
    lexErrorArr = resizedArray;
    delete[] resizedArray;
}

// Returns lexical error array pointer
char* Scanner::getLexErrors(void) const {
    return lexErrorArr;
}
// returns syntax error array pointer
string* Scanner::getSyntaxErrors(void) const {
    return syntaxErrorArr;
}


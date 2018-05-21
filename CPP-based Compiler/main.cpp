//
//  main.cpp
//  CPP-based Compiler
//
//  Created by Ryan Romanosky on 5/9/18.
//  Copyright © 2018 Ryan Romanosky. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include "FileHandler.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"

int main(int argc, const char * argv[]) {
    
    using namespace std;
    
    ifstream inp;
    ofstream outp;
    ofstream listp;
    bool eofReached = false;
    string token = "";
    FileHandler fh;
    Scanner sc;
    Parser ps;
    
    cout << "CPP-based Compiler\n";
    cout << "\tPreparing to open the file handler...\n\n" << endl;
    
    fh.OpenFiles(inp, outp, listp);
    
    
    while(!eofReached) {
        token = sc.GetToken(inp, outp, listp);
        cout << "Found token:\t" << token << endl;
        if (token == "") {
            eofReached = true;
        }
    }
    
    return 0;
}

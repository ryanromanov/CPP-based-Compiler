//
//  FileHandler.hpp
//  CPP-based Compiler
//
//  Created by Ryan Romanosky on 5/9/18.
//  Copyright © 2018 Ryan Romanosky. All rights reserved.
//

#ifndef FileHandler_hpp
#define FileHandler_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>


using namespace std;

class FileHandler {
private:
     // Variables
    int infiletype;
    int outfiletype;
    int listfiletype;
    int backupfiletype;
    int tempfiletype;
    
    string outputFileName;
    
    string outfileExtension;
    string infileExtension;
    string tempfileExtension;
    string backupfileExtension;
    string listfileExtension;
    
    bool exitApplication;
    
public:

    FileHandler();
    bool OpenInputFile(ifstream&);
    bool OpenOutputFile(ofstream&);
    bool OpenListingFile(ofstream&);
    bool BackupOutputFile(ofstream&, const string);
    bool CheckExtension(const string, const int);
    void ExtensionAdder(string&, const int);
    int OpenFiles(ifstream&, ofstream&, ofstream&);
    void CloseFiles(ifstream&, ofstream&, ofstream&);
};

#endif /* FileHandler_hpp */

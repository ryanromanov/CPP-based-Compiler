//
//  FileHandler.cpp
//  CPP-based Compiler
//
//  Created by Ryan Romanosky on 5/9/18.
//  Copyright © 2018 Ryan Romanosky. All rights reserved.
//

#include "FileHandler.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

FileHandler::FileHandler() {
    infiletype = 1;
    outfiletype = 2;
    listfiletype = 3;
    backupfiletype = 4;
    tempfiletype = 5;
    
    infileExtension = ".in";
    outfileExtension = ".out";
    listfileExtension = ".lis";
    tempfileExtension = ".temp";
    backupfileExtension = ".bak";
    
    outputFileName = "";
    
    exitApplication = false;
}

int FileHandler::OpenFiles(ifstream &inp, ofstream &outp, ofstream &listp) {
    bool inCheck = false;
    bool outCheck = false;
    bool listCheck = false;
    // loop through while loop
    // when all the checks are true, both they and the exitFile flag will be false
    // loop can end
    while ((!inCheck || !outCheck || !listCheck) && !exitApplication) {
        if (!inCheck) {
            inCheck = OpenInputFile(inp);
        }
        else if (!outCheck) {
            outCheck = OpenOutputFile(outp);
        }
        else if (!listCheck) {
            listCheck = OpenListingFile(listp);
        }
    }
    
    return 1;
}

bool FileHandler::OpenInputFile(ifstream &inp) {
    string tempFileName = "";
    bool extensionFound = false;
    bool fileOpened = false;
    
    while (!inp.is_open() && !exitApplication) {
        cout << "Please enter the name of the file you wish to open.\n";
        cout << "Typing \"quit\" will exit the application\n";
        cin >> tempFileName;
        // quit application if user types "quit" as the filename
        if (tempFileName == "quit") {
            exitApplication = true;
        }
        else if (tempFileName == "") {
            exitApplication = true;
        }
        else {
            extensionFound = CheckExtension(tempFileName, 1);
            if (!extensionFound) {
                ExtensionAdder(tempFileName, 1);
            }
            cout << "Attempting to open..." << tempFileName << endl;
            inp.open(tempFileName);
            if (!inp.is_open()) {
                cout << "** Error: File not found **\n";
            }
            else {
                fileOpened = true;
            }
        }
    }
    return fileOpened;
}
/************************************************************
 This function needs to do x amount of things
 1. It needs to check to see if entered name is quit
    - if so, it should quit the application
 2. then it needs to see if the entered name contains an extension
    - if so, good, if not, then we add the default (.out)
 3. It needs to see if that file already exists
    - to do so, we will try to open the file iwth ifstream(filename)
    - if this is true, we will close it and inform the user the filename already exists
        - we need to ask them if they want to overwrite it
            - if yes, then back it up to [outputname.ext].bak
            - if not, ask them if they want to quit or rename the file
    - if false, we are golden
************************************************************/
bool FileHandler::OpenOutputFile(ofstream &outp) {
    bool fileOpened = false;
    bool extensionFound = false;
    string tempFilename = "";
    bool overwrite = false;
    string overwriteString = "";
    string exitString = "";
    
    while (!outp.is_open() && !exitApplication) {
        cout << "Please enter a name for the output file.\n";
        cout << "Typing \"quit\" will exit the application\n";
        cin >> tempFilename;
        // quit application if name is "quit"
        if (tempFilename == "quit") {
            exitApplication = true;
        }
        else {
            // now we need to check if extension exists
            extensionFound = CheckExtension(tempFilename, 2);
            if (!extensionFound) {
                ExtensionAdder(tempFilename, 2);
            }
            // now we should have [filename.ext], so check if file exists
            // Make sure you CLOSE the file when you created the backup and everything
            if (ifstream (tempFilename)) {
                cout << "File " << tempFilename << " already exists.\n";
                cout << "Do you wish to overwrite this file?\n";
                cout << "(Enter \"y\" or \"n\")." << endl;
                cin  >> overwriteString;
                while (overwriteString != "y" && overwriteString != "n") {
                    cout << "Please enter \"y\" or \"n\".\n";
                    cin >> overwriteString;
                }
                if (overwriteString == "y") {
                    // here we call the function to backup the output file
                    // we also need to then create the output file
                    BackupOutputFile(outp, tempFilename);
                }
                // else ask if they want to quit, loop until they enter valid response
                else {
                    cout << "Do you wish to exit the application? (y/n)\n";
                    cin >> exitString;
                    while (exitString != "y" || exitString != "n") {
                        cout << "Please enter \"y\" or \"n\".\n";
                        cin >> exitString;
                    }
                    if (exitString == "y") {
                        exitApplication = true;
                    }
                }
            }
            // if exitApplication isn't true and they didn't choose no to overwriting
            // this means that it won't trigger if they said no to overwriting and no to quitting
            // should only trigger if they said yes to ovewriting, didn't need to overwrite
            if (!exitApplication && overwriteString != "n") {
                cout << "Attempting to create output file " << tempFilename << endl;
                outp.open(tempFilename);
                if (!outp.is_open()) {
                    cout << "** Error: Unable to open the output file **\n";
                }
                else {
                    cout << "Output file successfully created as \"" << tempFilename << "\".\n";
                    fileOpened = true;
                }
            }
        }
    }
    outputFileName = tempFilename;
    //
    return fileOpened;
}
/************************************************************
This function should create the listing file
- [outputfilename.ext].lis is the name of the listing file
- return true if created successfull
- return false otherwise
************************************************************/
bool FileHandler::OpenListingFile(ofstream &listp) {
    bool fileOpened = false;
    string listFilename = outputFileName;
    listFilename.append(".lis");
    listp.open(listFilename);
    if (listp.is_open()) {
        cout << "Successfully opened listing file as \"" << listFilename << "\". \n";
        fileOpened = true;
    }
    else {
        cout << "** Error: Problem opening listing file.\n";
        fileOpened = false;
    }
    return fileOpened;
}

/************************************************************
 This function should check to see if files have an extension
 - tempFileName is the filename sent from OpenInputFile
 - filetype is an int to tell us which filename (input, output, listing) is being sent
 -- 1 is input, 2 is output, 3 is listing (possible we won't use 3 but instead use [ouput_name].lis)
 - checks for extension,
 -- if found return true
 -- if not return false
************************************************************/
bool FileHandler::CheckExtension(const string tempFileName, const int filetype) {
    bool validName = false;
    size_t found;
    
    found = tempFileName.find_last_of(".");
    
    if (filetype == infiletype) {
        if (found == string::npos) {
            cout << "No extension found in input filename.\n" << endl;
            validName = false;
        }
        else {
            cout << "Infile Extension: " << tempFileName.substr(found) << "\n";
            validName = true;
        }
    }
    else if (filetype == outfiletype) {
        if (found == string::npos) {
            cout << "No extension found in ouput filename.\n" << endl;
            validName = false;
        }
        else {
            cout << "Output Extension: " << tempFileName.substr(found) << "\n";
            validName = true;
        }
    }
    return validName;
}
/************************************************************
This function will add an appropriate extension if one is not found in the files
 - tempfilename is the reference of the filename found
 - filetype is the type of file
 -- 1 is input, 2 is output, 3 is listing, 4 is temp, 5 is backup

************************************************************/
void FileHandler::ExtensionAdder(string& tempFileName, const int filetype) {
    if (filetype == infiletype ) {
        tempFileName.append(infileExtension);
        cout << "Default extension of \"" << infileExtension << "\" added\n";
    }
    else if (filetype == outfiletype) {
        tempFileName.append(outfileExtension);
        cout << "Default extension of \"" << outfileExtension << "\" added\n";
    }
    else if (filetype == listfiletype) {
        tempFileName.append(listfileExtension);
    }
    else if (filetype == tempfiletype) {
        tempFileName.append(tempfileExtension);
    }
    else if (filetype == backupfiletype) {
        tempFileName.append(backupfileExtension);
    }
    cout << "Filename is: " << tempFileName << endl;
}

/************************************************************
 BACKUP FUNCTION
 This function will backup the intended output file as [outputfilename.ext].bak.
 - then we will close the backup file and open the outputfile in overwrite mode
 - return true if everything ok
    - otherwise return false
************************************************************/
bool FileHandler::BackupOutputFile(ofstream& outp, const string outputFileName) {
    bool backupSucceeded = false;
    ofstream backup;
    string backupFilename = outputFileName;
    ifstream output_to_be_copied;
    char* readChar = NULL;
    readChar = new char;
    output_to_be_copied.open(outputFileName);
    
    backupFilename.append(".bak");
    backup.open(backupFilename);
    if (backup.is_open()) {
        cout << "Blank backup file successfully created as \"" << backupFilename << "\".\n";
        output_to_be_copied.get(*readChar);
        while (!output_to_be_copied.eof()) {
            backup.write(readChar, 1);
            cout << "Written char: " << *readChar << "\n";
            output_to_be_copied.get(*readChar);
        }
        backup.close();
        output_to_be_copied.close();
        cout << "Old Output file successfully backed up.\n";
        backupSucceeded = true;
    }
    else {
        cout << "** Error: There was a problem in creating the backup file. **\n";
        backupSucceeded = false;
    }
    
    return backupSucceeded;
}


//==========================================================================
// Name       : LogConverter.cpp
// Author     : Luke Hooker
// Version    : 0.0.0
// Copyright  : Not for reuse or distribution, with or without modification.
// Description: Converts a .txt log file into a .html log page
//==========================================================================

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctype.h>

using namespace std;

// Special exception base class
class SpecialExceptionBase: public std::runtime_error {
    public:
        SpecialExceptionBase( const std::string & message )
        : std::runtime_error( message ) { }
} ;
// Special exception macro
#define MAKE_SPECIAL_EXCEPTION( name, message ) \
class name: public SpecialExceptionBase { \
    public: \
        name() \
        : SpecialExceptionBase( message ) { } \
} ;
// exception definitions
MAKE_SPECIAL_EXCEPTION( error, "LogConverter has terminated due to an error" )
MAKE_SPECIAL_EXCEPTION( cantOpenFile, "Unable to open .txt file" )
MAKE_SPECIAL_EXCEPTION( cantWriteFile, "Unable to write .html file" )

void convertLogFile( const std::string & path ) {
    
    const char newLine[] = "\n";
    const char smallIndent[] = "  ";
    const char indent[] = "    ";    
    const char bigIndent[] = "      ";

    const std::string outPath(path.substr(0, path.length() - 4) + ".html");
    std::ofstream fOut(outPath);
    if (!fOut.is_open()) {
        throw cantWriteFile();
    }

    std::ifstream inFile(path);
    if (!inFile) {
        throw cantOpenFile();
    }

    // create/output html body and title tags
    fOut << "<html lang='en'>" << newLine << "<head> </head>" << newLine
        << smallIndent << "<body>" << newLine << indent
        << "<div style='white-space: pre; font-family: monospace;'>"
        << newLine << bigIndent << "<h1 style='text-align: center;'>Log File</h1>" << endl;
    
    // add break tags (<br>) after each line of log file
    std::string line;
    for(unsigned int lineNumber = 1; std::getline(inFile, line); lineNumber ++) {
        // cout << "lineNumber: "<< lineNumber << line << endl;
        fOut << bigIndent << line << "<br>" << endl;
    }

    // create/output closing HTML tags
    fOut << indent << "</div>" << newLine << smallIndent << "</body>" << newLine
        << "</html>" << endl;

    return;

}

int main( int argc, char* argv[] ) {
    // argument to pass: path of text file that needs converting
    const std::string txtPath(argv[1]);
    cout << "Converting log file at: " << txtPath << endl;

    try {
        convertLogFile(txtPath);
    }
    catch (const std::exception & ex) {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}
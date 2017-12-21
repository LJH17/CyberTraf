//==========================================================================
// Name       : LogAnalyser.cpp
// Author     : Luke Hooker
// Version    : 0.0.0
// Copyright  : Not for reuse or distribution, with or without modification.
// Description: Calculates the IP addresses of devices that exceed the
//              bandwidth limit set by the user
//==========================================================================

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

// Offenders structure
struct OffendersObject {
    std::vector<std::string> ip;
    std::vector<float> usage;
} ;

// DDOS structure
struct DdosObject {
    std::vector<std::string> incoming;
    std::vector<std::string> outgoing;
} ;

void createHTMLFile( const OffendersObject & offenders,
    const DdosObject & attackers,
    const std::string & path ) {
    
    // create offenders HTML file path
    const std::string outPath(path.substr(0, path.length() - 4) + "Offenders.html");
    cout << "Creating offender HTML file called: " << outPath << endl;

    // initialise ofstream at HTML file path
    std::ofstream fOut(outPath);
    if (!fOut.is_open()) {
        throw cantWriteFile();
    }

    // indent/newline constants
    const char newLine[] = "\n";
    const char smallIndent[] = "  ";
    const char indent[] = "    ";
    const char bigIndent[] = "      ";
    const char maxIndent[] = "        ";
    const char ul[] = "<ul>";
    const char xul[] = "</ul>";
    const char li[] = "<li>";
    const char xli[] = "</li>";
    const char bw[] = " (Usage: ";
    const char xbw[] = " Mbps)";

    // create/output html body and title tags
    fOut << "<html lang='en'>" << newLine << smallIndent << "<head>"
        << newLine << smallIndent << "</head>" << newLine << smallIndent
        << "<body>" << newLine << indent
        << "<div style='white-space: pre; font-family: monospace;'>" << newLine
        << bigIndent << "<h2 style='text-align: center;'>List of Offending IP Addresses</h2>"
        << newLine << bigIndent << ul << endl;

    // generate list of offending IP addresses using <li> tags
    for(unsigned int i = 0; i < offenders.ip.size(); i++) {
        fOut << maxIndent << li << offenders.ip[i] << bw << offenders.usage[i]
            << xbw << xli << endl;
    }
    if(offenders.ip.size() == 0) {
        fOut << maxIndent << li
            << "No IP addresses have offended in this period" << xli << endl;
    }
    fOut << bigIndent << xul << endl;

    // generate list of possible incoming DDOS sources
    fOut << bigIndent << "<h2 style='text-align: center;'>Possible Incoming DDOS Source</h2>"
        << newLine << bigIndent << ul << endl;
    for(unsigned int i = 0; i < attackers.incoming.size(); i++) {
        fOut << maxIndent << li << attackers.incoming[i] << xli << endl;
    }
    if(attackers.incoming.size() == 0) {
        fOut << maxIndent << li
            << "No possible incoming DDOS sources detected in this period"
            << xli << endl;  
    }
    fOut << bigIndent << xul << endl;    

    // generate list of possible outgoing DDOS source
    fOut << bigIndent << "<h2 style='text-align: center;'>Possible Outgoing DDOS Source</h2>"
        << newLine << bigIndent << ul << endl;
    for(unsigned int i = 0; i < attackers.outgoing.size(); i++) {
        fOut << maxIndent << li << attackers.outgoing[i] << xli << endl;
    }
    if(attackers.outgoing.size() == 0) {
        fOut << maxIndent << li
            << "No possible outgoing DDOS sources detected in this period"
            << xli << endl;  
    }
    fOut << bigIndent << xul << endl;    
    
    // create/output closing HTML tags
    fOut << indent << "</div>" << newLine
        << smallIndent << "</body>" << newLine << "</html>" << endl;

    return;

}

void analyseLogFile( const std::string & path ) {

    // create offender TXT file path
    // const std::string outPath(path.substr(0, path.length() - 4) + "Offenders.txt");
    // cout << "Creating offender file called: " << outPath << endl;
 
    // // initialise ofstream at TXT file path
    // std::ofstream fOut(outPath);
    // if (!fOut.is_open()) {
    //     throw cantWriteFile();
    // }

    // initialises ifstream at log file path
    std::ifstream inFile(path);
    if (!inFile) {
        throw cantOpenFile();
    }

    // create object of offenders
    OffendersObject offenders;
    // TESTING - mock network data
    offenders.ip.push_back("192.168.0.39");
    offenders.usage.push_back(30);
    offenders.ip.push_back("192.168.0.16");
    offenders.usage.push_back(25);

    // create object of ddos possibilities
    DdosObject attackers;
    // TESTING - mock attacker data
    attackers.incoming.push_back("ukhack-001.test.com");
    attackers.outgoing.push_back("192.168.0.16");

    // analyse each line for ip/bandwidth usage
    std::string line;
    for(unsigned int lineNumber = 1; std::getline(inFile, line); lineNumber ++) {
        // 
    }

    // genrate HTML file from offenders TXT file
    createHTMLFile(offenders, attackers, path);

    return;

}

int main( int argc, char* argv[] ) {
    // argument to pass: path of log file that needs analysing
    const std::string logPath(argv[1]);
    cout << "Analysing log file at: " << logPath << endl;

    try {
        analyseLogFile(logPath);
    }
    catch (const std::exception & ex) {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}

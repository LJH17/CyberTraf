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
#include <algorithm>

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
MAKE_SPECIAL_EXCEPTION( ipUsageMismatch, "IP usage data entries does not match the number of IP entries" )

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
    const std::string outPath(path.substr(0, path.length() - 4) + "-Offenders.html");
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

void analyseLogFile( const std::string & path, const float & limit ) {

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
    // offenders.ip.push_back("192.168.0.39");
    // offenders.usage.push_back(30);
    // offenders.ip.push_back("192.168.0.16");
    // offenders.usage.push_back(25);

    // create object of ddos possibilities
    DdosObject attackers;
    // TESTING - mock attacker data
    attackers.incoming.push_back("ukhack-001.test.com");
    attackers.outgoing.push_back("192.168.0.16");

    // analyse each line for ip/bandwidth usage
    std::string line;
    std::string ip("");
    std::string useString("");
    float use;

    std::vector<std::string> sourceIPs;
    std::vector<std::string> destinationIPs;
    std::vector<float> usage;

    bool IPread(false);
    bool useRead(false);

    for(unsigned int lineNumber = 1; std::getline(inFile, line); lineNumber ++) {
        IPread = false;
        if(lineNumber >= 4) {   // start line analysis from line 4
            if(line[0] == '-') {    // break from for loop if all IP addresses have been processed
                break;
            }
            if(lineNumber % 2 == 0) { // even line => source IP
                for(unsigned int charNumber = 0; charNumber < line.length(); charNumber ++) {
                    if(!IPread) {
                        if(charNumber >= 5) {   // beginning of IP chars
                            if(line[charNumber] != ' ') {  // if still reading IP
                                ip.push_back(line[charNumber]); // append char to IP string
                            }
                            else {
                                IPread = true;
                                sourceIPs.push_back(ip);
                                ip.clear();
                            }
                        }
                    }
                    else {  // source IP read
                        if(!useRead) {
                            if(charNumber >= 75) {  // start of BW avg over last 40 second data
                                if(line[charNumber] == 'M') {
                                    useRead = true;
                                    use = ::atof(useString.c_str());
                                }
                                else if(line[charNumber] == 'K') {
                                    useRead = true;
                                    use = ::atof(useString.c_str());
                                    use = use / 1000; // convert Kb to Mb
                                }
                                else if(line[charNumber] == 'b') {
                                    useRead = true;
                                    use = ::atof(useString.c_str());
                                    use = use / 1000000;    // convert b to Mb
                                }
                                else if(line[charNumber] != ' ') { // if char is not a space char
                                    useString.push_back(line[charNumber]);
                                }
                            }
                            if(useRead) {
                                useRead = false;    // reset useRead boolean flag to false
                                useString.clear();                                
                                break;  // escape for loop reading the line
                            }
                        }
                    }
                }
            }
            else {  // odd lines
                for(unsigned int charNumber = 0; charNumber < line.length(); charNumber ++) {
                    if(charNumber >= 5) {   // beginning of IP chars
                        if(line[charNumber] != ' ') {  // if still reading IP
                            ip.push_back(line[charNumber]); // append char to IP string
                        }
                        else {
                            destinationIPs.push_back(ip);
                            usage.push_back(use);
                            ip.clear();
                            break;
                        }
                    }
                }
            }
        }
    }

    // mismatch error if destination and usage entries size do not equal
    if(destinationIPs.size() != usage.size()) {
        throw ipUsageMismatch();
    }

    // TESTING line parsing/analysing
    // cout << "Source IPs" << endl;
    // for(unsigned int i = 0; i < sourceIPs.size(); i ++) {
    //     cout << sourceIPs[i] << endl;
    // }
    // cout << "Destination IPs and Usage" << endl;
    // for(unsigned int i = 0; i < destinationIPs.size(); i ++) {
    //     cout << destinationIPs[i] << "    " << usage[i] << endl;
    // }

    // check for possible incoming DDOS attack if all source IPs match
    bool sourceIPsMatch(true);
    for(unsigned int i = 0; i < sourceIPs.size() - 1; i ++) {
        if(sourceIPs[i] != sourceIPs[i + 1]) {
            sourceIPsMatch = false;
            break;
        }
    }
    if(sourceIPsMatch) {
        attackers.incoming.push_back(sourceIPs[0]);
    }

    // calculate BW usage of each IP
    std::vector<float> totalUsage;
    std::vector<std::string> IPList = destinationIPs;
    std::sort(IPList.begin(), IPList.end());
    auto it = std::unique(std::begin(IPList), std::end(IPList));
    IPList.erase(it, IPList.end());

    float BWCount;
    for(unsigned int i = 0; i < IPList.size(); i ++) {
        BWCount = 0;
        for(unsigned int j = 0; j < destinationIPs.size(); j ++) {
            if(destinationIPs[j] == IPList[i]) {
                BWCount += usage[j];
            }
        }
        totalUsage.push_back(BWCount);
    }

    // test total IP BW usage
    // cout << "Total BW usage by IP" << endl;
    for(unsigned int i = 0; i < IPList.size(); i ++) {
        // cout << IPList[i] << "    " << totalUsage[i] << " Mbps" << endl;
        if(totalUsage[i] >= limit) {
            offenders.ip.push_back(IPList[i]);
            offenders.usage.push_back(totalUsage[i]);
        }
    }

    // genrate HTML file from offenders TXT file
    createHTMLFile(offenders, attackers, path);

    return;

}

int main( int argc, char* argv[] ) {
    // argument to pass: path of log file that needs analysing
    const std::string logPath(argv[1]);
    cout << "Analysing log file at: " << logPath << endl;
    const float limit( ::atof(argv[2]));    // convert char* to float

    try {
        analyseLogFile(logPath, limit);
    }
    catch (const std::exception & ex) {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}

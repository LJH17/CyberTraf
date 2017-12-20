# Welcome to CyberTraf

File List:

1. pieChartGenerator.html  
    Converts a log file (currently manually chosen) into a pie chart

2. .gitignore  
    Allows git to intelligently ignore files that aren't needed in this repository.

3. cybertraf.sh  
    Accepts argument of Bandwidth and either a percentage or fixed level threshold  
    Runs the autoLogUploader.sh script which initialises `iftop`, uploads the generated log file to the FTP server, runs the `LogConverter` C++ program which converts the lgo to an HTML file, and uploads that to the FTP server
    
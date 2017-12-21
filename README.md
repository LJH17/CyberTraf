# CyberTraf

## A bandwidth-monitoring tool, delivered on a Raspberry Pi operating as a Wireless Access Point (WAP), with cyber data accessible through the CyberPortal web appplication

### File Descriptions:

1. pieChartGenerator.html  
    - Converts a log file (currently manually chosen) into a pie chart

2. barChartGenerator.html  
    - Converts a log file (currently manually chosen) into a bar chart

2. .gitignore  
    - Allows git to intelligently ignore files that aren't needed in this repository.

3. cybertraf.sh  
    - Accepts argument of Bandwidth and either a percentage or fixed level threshold 
    - Runs the `autoLogUploader.sh` script

4. autoLogUploader.sh  
    - Runs `iftop` to generate bandwidth usage data as a log .txt file
    - Uploads log file to CyberTraf FTP server using `ncftpput`
    - Runs `LogConverter` to convert .txt log file to .html file
    - Uplaods .html log file to FTP server
    - Runs `LogAnalyser` to detect offending IP addresses and create a .html file listing the offending IP addresses and their respective bandwidth usage
    - Uploads .html offenders file to FTP server

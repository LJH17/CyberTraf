#!/bin/bash
# 1. Create date/time stamp
# 2. Log for one minute using iftop
# 3. Upload log to FTP server using ncftpput
# Must occur indefinitely

# MUST ENTER [cntrl+C] TO STOP THIS LOOP
oldDateString=""
while true
do
    # 1.
    # get date
    currentDate=`date +%Y%m%d%H%M`
    # convert date to string and create paths
    dateString=$(printf "%12d" $currentDate)
    datePath="./iftop/logs/$dateString.txt"
    dateHTML="./iftop/logs/$dateString.html"
    offendersHTML="./iftop/logs/$dateString-Offenders.html"
   
     # 2.
    # check new file is different from old name
    if [ "$dateString" = "$oldDateString" ] #comment out if s param < 60
        then
            echo Date-match error. Check system time.
            break # leave infinite while loop
            # exit $?
    fi
    oldDateString="$dateString"
    
    # 3.
    # Run iftop to collect log of bandwidth usage on wireless access point    
    sudo iftop -i wlan0 -t -s 60 > "$datePath" # MUST CHANGE TO 60 SECONDS "-s 60"
    # Upload log .txt file to FTP server
    ncftpput -u cybertraf@adamfung.info -p cybertraf2017 -P 21 ftp.adamfung.info /data/ $datePath
    #if [ $? - ne 0 ]; then echo "Upload failed"; else echo "Upload successful"; fi
    
    #4.
    # convert .txt file to .html file, then upload to FTP server
    ./logParsing/LogConverter "$datePath"
    ncftpput -u cybertraf@adamfung.info -p cybertraf2017 -P 21 ftp.adamfung.info /data/ $dateHTML

    #5.
    # Call log analyser program to find offending IP addresses
    # Generate HTML file detailing offenders and upload to FTP server
    ./logParsing/LogAnalyser "$datePath" $1
    ncftpput -u cybertraf@adamfung.info -p cybertraf2017 -P 21 ftp.adamfung.info /data/ $offendersHTML
done

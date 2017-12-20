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
    #get date
    currentDate=`date +%Y%m%d%H%M`
    #convert date to string
    dateString=$(printf "%12d" $currentDate)
    datePath="iftop/logs/$dateString.txt"
    dateHTML="iftop/logs/$dateString.html"
    echo "Date string: $dateString        Old string: $oldDateString"
    # 2.
    # check new file is different from old name
    # if [ "$dateString" = "$oldDateString" ] #comment out if s param < 60
    #     then
    #         echo Date-match error. Check system time.
    #         break # leave infinite while loop
    #         # exit $?
    # fi
    oldDateString=$dateString
    sudo iftop -i wlan0 -t -s 5 > "./$datePath" # MUST CHANGE TO 60 SECONDS "-s 60"
    echo "$datePath"
    # 3.   
    ncftpput -u cybertraf@adamfung.info -p cybertraf2017 -P 21 ftp.adamfung.info /data/ ./$datePath
    #if [ $? - ne 0 ]; then echo "Upload failed"; else echo "Upload successful"; fi
    
    # convert .txt file to .html file, then upload to FTP server
    # passing path to .txt log file
    ./logParsing/LogConverter "$datePath"
    # upload HTML log file to FTP server
    ncftpput -u cybertraf@adamfung.info -p cybertraf2017 -P 21 ftp.adamfung.info /data/ ./$dateHTML    

    # call log file parser script
    # sh ./logParsing/logParser.sh $datePath
done

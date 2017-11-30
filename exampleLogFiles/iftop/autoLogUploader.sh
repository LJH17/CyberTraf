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
    dateString="$dateString.txt"
    echo $dateString
    # 2.
    # check new file is different from old name
    if [ "$dateString" = "$oldDateString" ]
        then
            echo Date-match error. Check system time.
            break # leave infinite while loop
    fi
    oldDateString=$dateString
    sudo iftop -i wlan0 -t -s 60 > $dateString # MUST CHANGE TO 60 SECONDS "-s 60"
    echo $dateString
    # 3.
    ncftpput -u cybertraf@adamfung.info -p cybertraf2017 -P 21 ftp.adamfung.info /data/ /home/pi/workspace/CyberTraf/exampleLogFiles/iftop/$dateString
    #if [ $? - ne 0 ]; then echo "Upload failed"; else echo "Upload successful"; fi
done

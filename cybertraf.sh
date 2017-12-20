#!/bin/bash

# accept user parameters

# 1. BW (Define) -B <BW>
# 2. Percentage level -P <% level>
# or 3. Fixed level -F <fixed level>

bandwidth=0
level=0
percentage_level=0
valid_level="false"

while getopts ":B:P:L:" opt; do
    case ${opt} in
        B )
            bandwidth=$OPTARG
            ;;
        P )
            percentage_level=$OPTARG
            ;;
        L )
            level=$OPTARG
            ;;
        \? ) echo "Invalid argument suppplied: $OPTARG"
             echo "Acceptable command line parameters:"; echo "-B <Bandwidth> -P <% Level> -L <Fixed Level>"
             exit $?
             ;;
        : )
            echo "Please provide an argument"
            exit $?
            ;;
    esac
done

if [ "$level" -gt "$bandwidth" ]; then
    echo "Level provided ($level) cannot be greater than the bandwidth ($bandwidth)"
    exit $?
fi

if [ "$percentage_level" -gt "100" ] || [ "$percentage_level" -lt "0" ]; then
    echo "Percentage level provided ($percentage_level) cannot be greater than 100% or less than 0%"
    exit $?
fi

echo "Welcome to CyberTraf\n"
echo "Arguments supplied:"
echo "         bandwidth: $bandwidth Mb"

if [ "$percentage_level" -ne "0" ]; then
    echo "  percentage level: $percentage_level% [% of Bandwidth used as limit]"    
    level=$(awk "BEGIN {printf \"%.2f\",${percentage_level}/100*${bandwidth}}")
    echo "             level: $level Mb [Any device exceeding this level over any 60 seconds is offending]"    
    echo "(Limit based on $percentage_level% of $bandwidth Mb = $level Mb)"
else
    echo "             level: $level Mb [Any device exceeding this level over any 60 seconds is offending]"
fi

echo "\nPlease enter [CNTRL + C] to end CyberTraf"

#sh ./iftop/autoLogUploader.sh

#!/bin/bash

# accept user parameters

# 1. BW (Define) -B <BW>
# 2. Percentage level -P <% level>
# or 3. Fixed level -F <fixed level>

bandwidth=0
level=0
percentage_level=0

while getopts ":B:P:F:" opt; do
    case ${opt} in
        B )
            bandwidth=$OPTARG
            ;;
        P )
            percentage_level=$OPTARG
            ;;
        F )
            level=$OPTARG
            ;;
        \? ) echo "Invalid argument suppplied: $OPTARG"
             echo "Acceptable command line parameters:"; echo "-B <Bandwidth> -P <% level> -F <Fixed Level>"
             exit $?
             ;;
        : )
            echo "Please provide an argument"
            exit $?
            ;;
    esac
done

if [ "$percentage_level" -ne "0" ]; then
    # percentage_multiplier=$(( $percentage_level * $toDecimal ))
    level=$(awk "BEGIN {printf \"%.2f\",${percentage_level}/100*${bandwidth}}")
    echo "Limit based on $percentage_level% of $bandwidth: $level"
fi

echo "Arguments supplied:"
echo "  bandwidth: $bandwidth Mb"
echo "  level: $level Mb [Any device exceeding this level over 60 seconds is offending]"
echo "  percentage level: $percentage_level% [% of Bandwidth used as limit]"
echo "Please enter [CNTRL + C] to end CyberTraf"

#sh ./iftop/autoLogUploader.sh

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

echo "Arguments supplied:"
echo "  bandwidth: $bandwidth"
echo "  level: $level"
echo "  percentage level: $percentage_level"

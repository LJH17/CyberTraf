#!/bin/bash

echo "Converting .txt log file at path $1 to a .html file"

# run C++ program to convert .txt file to .html, then upload to FTP server

./logParsing/LogConverter "$1"

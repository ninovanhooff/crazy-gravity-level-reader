#!/bin/bash
set -e # exit on error
FILENAME="nino4"
clear
echo "--- COMPILING cglreader"
./compile.sh
echo "--- READING specials & levelProps"
./run.sh $FILENAME
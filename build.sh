#!/bin/bash
set -e # exit on error
FILENAME="User001"
clear
echo "--- COMPILING cglreader"
./compile.sh
echo "--- READING specials & levelProps"
./run.sh $FILENAME
echo "--- Copying levels to game sources"
cp -R ./lua-levels/* ../CrazyGravityPlaydate/Source/levels
echo "--- Opening Simulator"
pdc ../CrazyGravityPlaydate/Source /tmp/cgpd.pdx && open /tmp/cgpd.pdx
#!/bin/bash
./cglreader $1
echo "### READING brick data and WRITING final lua file and RENDERING PNG"
/Applications/love.app/Contents/MacOS/love /Users/ninovanhooff/PlaydateProjects/GravityExpressEditor $1
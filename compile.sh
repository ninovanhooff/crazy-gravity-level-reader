FILENAME="nino3"
clear; gcc -std=c99 -Wall -o cglreader *.c && ./cglreader $FILENAME && /Applications/love.app/Contents/MacOS/love /Users/ninovanhooff/PlaydateProjects/GravityExpressEditor $FILENAME
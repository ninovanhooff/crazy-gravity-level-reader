FILENAME="nino3"
clear
echo "--- READING specials & levelProps"
gcc -std=c99 -Wall -o cglreader *.c
./cglreader $FILENAME
echo "--- READING brick data and WRITING final lua file and RENDERING PNG"
/Applications/love.app/Contents/MacOS/love /Users/ninovanhooff/PlaydateProjects/GravityExpressEditor $FILENAME
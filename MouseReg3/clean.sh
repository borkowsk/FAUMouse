#!/bin/bash
# **FAUMouse3 is a package for recording mouse movement on the screen via the web**
#
#__THIS IS BETA VERSION FOR PRIVATE USAGE__
#
#2020 (c) Wojciech Borkowski
#
source "screen.ini"
echo -e $COLOR1"All non-needed files in '" `pwd` "' will be cleaned"$COLOR2
echo -e "Press Ctrl-C in 4 seconds!"$NORMCO
sleep 4
rm -f service.log
rm -f .qtcreator.err
rm -f *.*~
rm -f IPs.txt
rm -f stop.sh
rm -f src/*.cmake
rm -f src/CMakeCache.txt
rm -f src/CMakeLists.txt.user
rm -f src/Makefile
rm -R -f src/CMakeFiles
rm -R -f build-*
pushd fasada-core/
./clean.sh
popd

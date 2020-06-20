#!/bin/bash
BROWSER=firefox #Select proper LOCAL browser
MYADDRES=localhost #For debbuging only!
MYPORT=8889
#MYADDRES=192.168.1.100 #Select the IP proper for testing
#MYPORT=8989    #Select the proper PORT for testing, when 8989 is not available

if [ $# -eq 2 ]
then
 #echo "2 arguments supplied"
 MYADDRES="$1"
 MYPORT="$2"    
fi

#TERMINAL CONFIG
if [ ! -e "screen.ini" ]
then
  cp  "fasada-core/lib/fasada/_config_template/screen.ini" ./
fi
source "screen.ini"

#REAL JOB
echo -e $COLOR1 STARTING SERVICE $COLOR2
(./src/regmouse3 > service.log 2>&1 &)\
 && (sleep 1 )\
 && (./fasada-core/wwwserver  $MYADDRES $MYPORT ./public/ &)\
 && (sleep 1 )\
 && ($BROWSER http://$MYADDRES:$MYPORT/ >> service.log 2>&1 &)\
 && echo "$BROWSER http://$MYADDRES:$MYPORT/!!!!" > stop.sh \
 && chmod +x stop.sh \
 && echo -e $COLOR1 For close send $COLOR2\
            "http://$MYADDRES:$MYPORT/!!!!"\
            $COLOR1 from $BROWSER $NORMCO "\n" \
            $COLOR1 Or use $COLOR2 stop.sh $COLOR1 in this directory $NORMCO


#!/bin/bash
MYADDRES=localhost #For debuging only!
BROWSER=firefox #Select proper LOCAL browser
#MYADDRES=192.168.1.100 #Select the proper IP

#TERMINAL CONFIG
if [ ! -e "screen.ini" ]
then
  cp  "fasada-core/lib/fasada/_config_template/screen.ini" ./
fi
source "screen.ini"

#REAL JOB
echo -e $COLOR1 STARTING SERVICE $COLOR2
(./fasada-core/hello > service.log 2>&1 &)\
 && (sleep 1 )\
 && (./fasada-core/wwwserver  $MYADDRES 8888 ./public/ &)\
 && (sleep 1 )\
 && ($BROWSER http://$MYADDRES:8888/ >> service.log 2>&1 &)\
 && echo -e $COLOR1 For close send $COLOR2\
            "http://$MYADDRES:8888/!!!!"\
            $COLOR1 from $BROWSER $NORMCO

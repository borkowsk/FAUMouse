#!/bin/bash
#TERMINAL CONFIG
if [ ! -e "screen.ini" ]
then
  cp  "fasada-core/lib/fasada/_config_template/screen.ini" ./
fi
source "screen.ini"

#REAL JOB
pushd fasada-core
echo -e $COLOR1 GIT JOB $NORMCO \
&& git checkout master \
&& git pull \
&& echo -e $COLOR1 CMAKE JOB $COLOR2 \
&& cmake . \
&& echo -e $COLOR1 MAKE JOB $NORMCO \
&& make \
&& echo -e $COLOR1 AVAILABLE IPs $NORMCO \
&& ifconfig | tee "../IPs.txt" \
&& echo -e $COLOR1 DONE $NORMCO
popd


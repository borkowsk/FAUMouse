#!/bin/bash
FAUMOUSEVERSION="3 (2020)"

#TERMINAL CONFIG
if [ ! -e "screen.ini" ]
then
  cp  "fasada-core/lib/fasada/_config_template/screen.ini" ./
fi
source "screen.ini"

#REAL JOB
echo -e $COLOR2 GIT $COLOR1 FOR FAU-MOUSE  $NORMCO \
&& git checkout master \
&& git pull \
&& echo -e $COLOR1 \
&& pushd fasada-core \
&& echo -e $COLOR2 GIT $COLOR1 FOR FASADA  $NORMCO \
&& git checkout master \
&& git pull \
&& echo -e $COLOR2 CMAKE $COLOR1 FOR FASADA $COLOR2 \
&& cmake . \
&& echo -e $COLOR2 MAKE $COLOR1 FOR FASADA $NORMCO \
&& make \
&& echo -e $COLOR1 \
&& popd \
&& pushd src \
&& echo -e $COLOR2 CMAKE $COLOR1 FOR FAU-MOUSE version $FAUMOUSEVERSION $COLOR2 \
&& cmake . \
&& echo -e $COLOR2 MAKE $COLOR1 FOR FAU-MOUSE version $FAUMOUSEVERSION $COLOR2 \
&& make \
&& echo -e $COLOR1 \
&& popd \
&& echo -e $COLOR1 AVAILABLE IPs $NORMCO \
&& ifconfig | tee "IPs.txt" | grep -E --color "([0-9]{1,3}\.){3}[0-9]{1,3}" \
&& echo -e $COLOR1 USE $NORMCO "start.sh" $COLOR1 FOR RUN THE SERVICE LOCALLY \
&& echo -e $COLOR1 USE $NORMCO "start.sh" "IP" "PORT" $COLOR1 FOR RUN THE SERVICE IN THE NETWORK \
&& echo -e $COLOR1 DONE $NORMCO \


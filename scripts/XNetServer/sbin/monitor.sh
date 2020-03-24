#!/bin/bash

DIR=$(cd `dirname $0`;cd ..;pwd)
PROGRAM=`echo ${DIR}|awk -F/ '{print $NF}'`

function processCount(){
        count=$(ps -ef | grep -w "${DIR}/bin/${PROGRAM}" | grep -v 'grep' |wc -l)
        echo $count
}


function check(){
        execPath=${DIR}/sbin/${PROGRAM}.sh
        count=`processCount`
        now=$(date '+%Y-%m-%d %H:%M:%S')
        if [ $count -eq 0 ];then
                sh $execPath start
#
                count=`processCount`
                if [ $count -eq 1 ];then
                        echo "$now ${PROGRAM} start success"
                else
                        echo "$now ${PROGRAM} start fail"
                fi
        fi
}

check

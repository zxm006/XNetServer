#!/bin/bash
server_name="XNetServer"
server_path="/usr/local/XNetServer/bin/XNetServer"

function start() {
  eval  "nohup ${server_path} > /dev/null 2>&1 &"
sleep 2

#echo "pwd:$(pwd)"
DIR=$(cd `dirname $0`;cd ..;pwd)

cd  $DIR
#echo "pwd:$(pwd)"

cmd=`ps -fe|grep "$server_name" |grep -v "grep" | grep  -v "start" | grep  -v "restart"`
#echo "------  $cmd"
if [ -z "$cmd" ]
then
echo "$server_name start failed"
else
echo "$server_name start success"
fi


ps -fe|grep $server_name |grep -v grep
if [ $? -ne 0 ]
then
echo "$server_name start failed"
else
echo "$server_name start success"
fi
}

function stop() {


cmd=`ps -fe|grep "$server_name" |grep -v "grep" | grep  -v "stop" | grep  -v "restart"`
#echo "------  $cmd"
if [ -z "$cmd" ]
then
echo "$server_name stop no process"
 exit
fi


ID=`ps -ef | grep "$server_name" | grep -v "$0" | grep -v "grep" | awk '{print $2}'`
#echo $ID

for id in $ID
do
kill -9 $id
done

cmd=`ps -fe|grep "$server_name" |grep -v "grep" | grep  -v "stop" | grep  -v "restart"`
#echo "------  $cmd"
if [ -z "$cmd" ]
then
echo "$server_name stop success"
else
echo "$server_name stop failed"
fi
}

function restart() {
stop
start
  echo "$server_name restart"
}

case "$1" in
start )

start

;;
stop )

stop

;;
restart )

restart

;;
* )

echo "no command"

;;
esac


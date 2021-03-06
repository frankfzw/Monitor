#!/bin/bash

PASSWORD=$1 
MONITOR_HOME=$2

usage="startMonitor Password MonitorHome"
if [ $# -le 0 ]; then
  echo $usage
  exit 1
fi
#start the monitor on the master at first
echo $1 | sudo -S screen -d -m -S monitor $MONITOR_HOME/monitor $MONITOR_HOME
# Where to start the script, see hadoop-config.sh
# (it set up the variables based on command line options)
if [ "$HADOOP_SLAVE_NAMES" != '' ] ; then
  SLAVE_NAMES=$HADOOP_SLAVE_NAMES
else
  SLAVE_FILE=${HADOOP_SLAVES:-${HADOOP_CONF_DIR}/slaves}
  SLAVE_NAMES=$(cat "$SLAVE_FILE" | sed  's/#.*$//;/^$/d')
fi

# start the daemons
for slave in $SLAVE_NAMES ; do
 echo $1 | ssh hduser@$slave sudo -S screen -d -m -S monitor $MONITOR_HOME/monitor $MONITOR_HOME 
 if [ "$HADOOP_SLAVE_SLEEP" != "" ]; then
   sleep $HADOOP_SLAVE_SLEEP
 fi
done

wait

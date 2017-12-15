#!/bin/bash

if [ $# -lt 2 ]
then
    echo "Usage: ./Master-Limit.sh Num_Worker Rate(Mbps)"
    echo "Ex: ./Master-Limit.sh 5 20"
    exit
fi

echo "// Rate limit all Workers"
for (( i = 1; i <= $1; i++ ))
do
    sudo -u ubuntu ssh n$i sudo tc qdisc del dev eth0 root
    if [ $2 -gt 0 ]
    then
	sudo -u ubuntu ssh n$i sudo tc qdisc add dev eth0 handle 1: root htb default 11
	sudo -u ubuntu ssh n$i sudo tc class add dev eth0 parent 1: classid 1:1 htb rate "$2"mbit
	sudo -u ubuntu ssh n$i sudo tc class add dev eth0 parent 1:1 classid 1:11 htb rate "$2"mbit
    fi
done

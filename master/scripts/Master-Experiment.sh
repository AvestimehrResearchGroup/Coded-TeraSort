#!/bin/bash

if [ $# -lt 4 ]
then
    echo "Usage: ./Master-Experiment.sh K r B(GB) Rate(Mbps)"
    echo "Ex: ./Master-Experiment 16 3 10 400"
    exit
fi

echo "// Experiment K = $1 r = $2, B = $3 GB, m3.large"
echo "// Rate limit all Workers at Rate $4 Mbps"
host="localhost"
for (( i = 1; i <= $1; i++ ))
do
    host="$host,n$i"
    sudo -u ubuntu ssh n$i sudo tc qdisc del dev eth0 root
    if [ $4 -gt 0 ]
    then
	sudo -u ubuntu ssh n$i sudo tc qdisc add dev eth0 handle 1: root htb default 11
	sudo -u ubuntu ssh n$i sudo tc class add dev eth0 parent 1: classid 1:1 htb rate "$4"mbit
	sudo -u ubuntu ssh n$i sudo tc class add dev eth0 parent 1:1 classid 1:11 htb rate "$4"mbit
    fi
done


for (( i = 1; i <= 5; i++ ))
do
    echo ""
    echo ""
    echo "// CodedTeraSort $i"    
    mpirun -host $host ./CodedTeraSort
done


for (( i = 1; i <= 5; i++ ))
do
    echo ""
    echo ""
    echo "// TeraSort $i"    
    mpirun -host $host ./TeraSort
done

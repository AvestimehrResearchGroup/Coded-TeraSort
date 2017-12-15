#!/bin/bash

if [ $# -lt 2 ]
then
    echo "Usage: ./Master-Run-CodedTeraSort.sh K Rate(Mbps)"
    echo "Ex: ./Master-Experiment 16 400"
    exit
fi

make

echo "// Experiment K = $1"
echo "// Rate limit: $2 Mbps"
host="localhost"
for (( i = 1; i <= $1; i++ ))
do
    host="$host,n$i"
    sudo -u apac ssh -t n$i sudo tc qdisc del dev eth1 root
    if [ $2 -gt 0 ]
    then
	sudo -u apac ssh -t n$i sudo tc qdisc add dev eth1 handle 1: root htb default 11
	sudo -u apac ssh -t n$i sudo tc class add dev eth1 parent 1: classid 1:1 htb rate "$2"mbit
	sudo -u apac ssh -t n$i sudo tc class add dev eth1 parent 1:1 classid 1:11 htb rate "$2"mbit
    fi
    scp ./CodedTeraSort n$i:TeraSort/    
done


echo ""
echo ""
echo "// Run TeraSort"    
mpirun -mca btl ^openib --mca btl_tcp_if_include eth1 --mca oob_tcp_if_include eth1 -host $host --mca plm_rsh_no_tree_spawn 1 ./CodedTeraSort

for (( i = 1; i <= $1; i++ ))
do
    scp apac@n$i:TeraSort/Output/countIPs-C.txt ~/TeraSort
    mv countIPs-C.txt countIPs-C_$i.txt
    cat countIPs-C_$i.txt >> tempOutput-C.txt
done

mv tempOutput-C.txt ~/TeraSort/Intermediate/countIPs-C.txt

g++ -std=c++11 filterResult.cpp -o filterResult
./filterResult ~/TeraSort/Intermediate/countIPs-C.txt ~/TeraSort/Output/result-C.txt 50

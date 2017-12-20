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
    ssh -t n$i sudo tc qdisc del dev eth0 root
    if [ $2 -gt 0 ]
    then
	ssh -t n$i sudo tc qdisc add dev eth0 handle 1: root htb default 11
	ssh -t n$i sudo tc class add dev eth0 parent 1: classid 1:1 htb rate "$2"mbit
	ssh -t n$i sudo tc class add dev eth0 parent 1:1 classid 1:11 htb rate "$2"mbit
    fi
    scp ./CodedTeraSort n$i:/root/TeraSort/    
done


echo ""
echo ""
echo "// Run TeraSort"    
mpirun --allow-run-as-root -mca btl ^openib --mca btl_tcp_if_include eth0 --mca oob_tcp_if_include eth0 -host $host --mca plm_rsh_no_tree_spawn 1 ./CodedTeraSort

for (( i = 1; i <= $1; i++ ))
do
    scp n$i:/root/TeraSort/Output/countIPs-C.txt ~/TeraSort
    mv countIPs-C.txt countIPs-C_$i.txt
    cat countIPs-C_$i.txt >> tempOutput-C.txt
done

mv tempOutput-C.txt ~/TeraSort/Intermediate/countIPs-C.txt

# g++ -std=c++11 filterResult.cpp -o filterResult
./filterResult ~/TeraSort/Intermediate/countIPs-C.txt ~/TeraSort/Output/result-C.txt $3

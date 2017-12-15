#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./Master-InputPlacement.sh Num_Worker"
    echo "Ex: ./Master-InputPlacement.sh 5"
    exit
fi

make

host="localhost"
for (( i = 1; i <= $1; i++ ))
do
    echo $i
    host="$host,n$i"
    sudo -u apac ssh -t n$i sudo tc qdisc del dev eth1 root
    #ssh n$i 'rm -rf ~/TeraSort/Input; mkdir -p ~/TeraSort/Input'
    ssh n$i 'rm -rf ~/TeraSort/Intermediate; mkdir -p ~/TeraSort/Intermediate'
    #ssh n$i 'rm ~/TeraSort/Input/*-*'
    scp ./InputPlacement n$i:TeraSort/
done

mpirun -mca btl ^openib --mca btl_tcp_if_include eth1 --mca oob_tcp_if_include eth1 -host $host --mca plm_rsh_no_tree_spawn 1 ./InputPlacement
mpirun -mca btl ^openib --mca btl_tcp_if_include eth1 --mca oob_tcp_if_include eth1 -host $host --mca plm_rsh_no_tree_spawn 1 ./InputPlacement code

    

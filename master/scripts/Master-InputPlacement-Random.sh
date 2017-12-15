#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./Master-InputPlacement-Random.sh Num_Worker"
    echo "Ex: ./Master-InputPlacement-Random.sh 5"
    exit
fi

make

host="localhost"
for (( i = 1; i <= $1; i++ ))
do
    echo $i
    host="$host,n$i"
    sudo -u ubuntu ssh n$i sudo tc qdisc del dev eth0 root
    ssh n$i 'rm -rf ~/TeraSort/Input; mkdir -p ~/TeraSort/Input'
    #ssh n$i 'rm ~/TeraSort/Input/*-*'
    scp ./InputPlacementRandom n$i:TeraSort/
done

mpirun -mca btl ^openib -host $host --mca plm_rsh_no_tree_spawn 1 ./InputPlacementRandom code

    

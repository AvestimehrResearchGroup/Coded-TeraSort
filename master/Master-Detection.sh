#!/bin/bash

while IFS=':' read -ra ADDR; do for i in "${ADDR[@]}"; do echo $i >> ips.txt; done; done <<< "$NODE_IPS"

./Master-Network.sh 3 ips.txt  # 3 is the number of workers
                               # ips.txt lists the IP of the workers

./Master-PrepareInput.sh Input/data.txt  # the detected track, in a form of
                                         # {time, srcIP, destIP, ... }

./Master-InputPlacement.sh 3  # 3 is the number of workers

if [ "$1" = "coded" ]
then
    ./Master-Run-CodedTeraSort.sh 3 50
else
    ./Master-Run-TeraSort.sh 3 50  # 3 is the number of workers
                                       # 50 is the threshold of IP frequency
                                       # Only IPs detected by more than 50 times will
                                       #   be listed in Output/result.txt 
fi

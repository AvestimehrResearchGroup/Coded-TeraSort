#!/bin/bash

#!./Master-Initialization.sh

./Master-Network.sh 3 $2

./Master-PrepareInput.sh $3

./Master-InputPlacement.sh 3

if [ "$1" = "coded" ]
then
    ./Master-Run-CodedTeraSort.sh 3 100
else
    ./Master-Run-TeraSort.sh 3 100 
fi




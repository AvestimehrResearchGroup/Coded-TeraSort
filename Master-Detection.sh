#!/bin/bash

#!./Master-Initialization.sh

./Master-Network.sh 3

./Master-PrepareInput.sh

./Master-InputPlacement.sh 3

./Master-Run-TeraSort.sh 3 100



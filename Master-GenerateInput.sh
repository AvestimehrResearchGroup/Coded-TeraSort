#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./Master-GenerateInput.sh Num_Key"
    echo "Ex: ./Master-GenerateInput.sh 1000000"
    exit
fi

export JAVA_HOME=/usr/lib/jvm/java-9-openjdk-amd64/
rm -rf ~/TeraSort/GenInput
~/hadoop-2.8.1/bin/hadoop jar ~/hadoop-2.8.1/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.8.1.jar teragen $1 ~/TeraSort/GenInput
rm -rf ~/TeraSort/Input/*
mv ~/TeraSort/GenInput/part-m-00000 ~/TeraSort/Input/Input$1
ln -s -T ~/TeraSort/Input/Input$1 ~/TeraSort/Input/Input$1-C

#!/bin/bash

# g++ -std=c++11 extractIPs.cpp -o extractIPs

./extractIPs $1 /root/TeraSort/Intermediate/srcIPs.txt

cp /root/TeraSort/Intermediate/srcIPs.txt /root/TeraSort/Intermediate/srcIPs-C.txt

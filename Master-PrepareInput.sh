#!/bin/bash

g++ -std=c++11 extractIPs.cpp -o extractIPs

./extractIPs $1 ~/TeraSort/Intermediate/srcIPs.txt

cp ~/TeraSort/Intermediate/srcIPs.txt ~/TeraSort/Intermediate/srcIPs-C.txt 
                                                          

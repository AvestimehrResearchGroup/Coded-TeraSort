#!/bin/bash

g++ -std=c++11 extractIPs.cpp -o extractIPs

./extractIPs $1 

cp ~/TeraSort/Input/srcIPs.txt ~/TeraSort/Input/srcIPs-C.txt 
                                                          

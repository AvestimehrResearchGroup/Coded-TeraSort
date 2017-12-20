#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./Master-Network.sh Num_Worker"
    echo "Ex: ./Master-Network.sh 5"
    exit
fi

echo "// Backup hosts"
sudo cp /etc/hosts /etc/hosts_backup

echo "// Setup Worker IP"
#!for (( i = 1; i <= $1; i++ ))
#!do
#!    echo "n$i's IP:"
#!    read nip
#!    echo "$nip n$i" | sudo tee --append /etc/hosts
    #! sudo echo "$nip n$i" >> /etc/hosts
#!done

numWorker = 0
while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "Text read from file: $line"
    numWorker=`expr $numWorker + 1`
    echo "$line n$numWorker" | sudo tee --append /etc/hosts
done < "$2"


echo "// Generate RSA key for seamless ssh"
bash -c "ssh-keygen -t rsa -f ~/.ssh/id_rsa -N ''"

echo "// Setup all Workers"
for (( i = 1; i <= $1; i++ ))
do
    bash -c "cat ~/.ssh/id_rsa.pub | sshpass -p 'zhifeng' ssh n$i -o StrictHostKeyChecking=no 'cat - >> ~/.ssh/authorized_keys'"
    ssh n$i 'mkdir -p TeraSort/Output'
    #ssh n$i 'sudo chown apac:apac ~/TeraSort'
    # ssh -t n$i 'sudo apt-get -q update; sudo apt-get -q install openmpi-bin libopenmpi-dev -y'
done

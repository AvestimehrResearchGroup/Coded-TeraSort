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
for (( i = 1; i <= $1; i++ ))
do
    echo "n$i's IP:"
    read nip
    echo "$nip n$i" | sudo tee --append /etc/hosts
    #! sudo echo "$nip n$i" >> /etc/hosts
done

echo "// Generate RSA key for seamless ssh"
sudo -u apac bash -c "ssh-keygen -t rsa -f ~/.ssh/id_rsa"

echo "// Setup all Workers"
for (( i = 1; i <= $1; i++ ))
do
    sudo -u apac bash -c "cat ~/.ssh/id_rsa.pub | ssh n$i 'cat - >> ~/.ssh/authorized_keys'"
    sudo -u apac ssh n$i 'mkdir -p TeraSort/Output'
    sudo -u apac ssh n$i 'sudo chown apac:apac ~/TeraSort'
    sudo -u apac ssh -t n$i 'sudo apt-get -q update; sudo apt-get -q install openmpi-bin libopenmpi-dev -y'
done

Overview
============

This repo is a complete Docker + Kubernetes implementation of coded Tera-Sort based
anomaly detection system. When executed, it extracts the source IPs from the
detection trace, and returns the source IPs that are too active, which are
likely to be anomaly.

Build the Docker Images
==========================

Build the master
------
Enter the **/master** folder of this repo, and run:
~~~
docker build -t master:v0.03 .
docker tag master:v0.03 fishermanymc/master:v0.03
docker push fishermanymc/master:v0.03
~~~

Build the worker
------
Enter the **/worker** folder of this repo, and run:
~~~
docker build -t worker:v0.03 .
docker tag master:v0.03 fishermanymc/worker:v0.03
docker push fishermanymc/worker:v0.03
~~~

Note: If you use other mastre & worker images/tags,
please don't forget to also update them in
**k8s_coding.py** in the main folder as well.

The fake master
---------------
In **/fake_master** folder of this repo, we provide a Python script **teraSort.py**.
It can be executed by any Kubernetes node to remotely run the detection algorithm at the master. The script only needs to know the IP address of the master after the master service/deployment is launched, which we now cover.

(For the reason why we consider this fake master,
please refer to the TODO section at the end of
this article.)

Launch the Service & Deployment
==================

   1. Log into a Kubernetes cluster;

   2. select 4 nodes;

   3. Place their name (e.g. ubuntu-2gb-nyc2-02) to **nodes_coded.txt**. The
      format should be the same as the existing **nodes_coded.txt**. It lookes
      like:
~~~
nerv1 ubuntu-2gb-nyc2-02 root zhifeng
nerv2 ubuntu-2gb-nyc2-03 root zhifeng
nerv3 ubuntu-2gb-sfo1-01 root zhifeng
nerv4 ubuntu-2gb-sfo1-02 root zhifeng
~~~

   4. Run:
~~~
python3 k8s_coding.py
~~~

To see if it is succeeded, run:
~~~
kubectl get po --namespace=fisher-coding
~~~

You should see something like:
~~~
NAME                     READY     STATUS    RESTARTS   AGE
nerv1-3622045861-1g0l2   1/1       Running   0          22s
nerv2-1659898038-1zxc1   1/1       Running   0          22s
nerv3-1728448691-jvpq8   1/1       Running   0          22s
nerv4-2022443190-c9qg7   1/1       Running   0          22s
~~~

Now nerv1 is the master, and nerv2, 3, 4 are the workers.

Launch the TeraSort-based Detection System
===========================

Pre-requirement:
----------------
Before launching the detection system, make sure that there is a **data.txt**
file available in nerv1's /root/TeraSort/Input folder. This file looks like:
~~~
1312967064.406378 147.32.80.9 53 147.32.84.165 1025 125 U -
1312967064.406786 147.32.84.165 137 147.32.84.255 137 78 U -
...
~~~

each row records a packet transfer from a source IP to a destination IP,
where the second part of each row is the source IP that we are interested in.

Note: We assume that in-line parts are separated by a space, i.e.,  ' '. If you
use a different seperator, please either reformat your data, or
change the *SEP* macro in **extractIPs.cpp** according.
   

Parameter setting
------------------
   * anomaly threshold: if the number of times an IP appears is greater than this
     threshold, we will treat this IP as an anomaly. This parameter can be set
     at **Master-Dection.sh**
   * worker communication bandwidth limit: This parameter is used to throttle
     the communication speed between workers. It can be set at **/worker/Dockerfile**
     before building the worker's Docker image.
   * IP address length. TeraSort requires equal-length keys. Since the maximum
   possible length for IPv4 is 16 (including the dots), for short IPs we append dummy
   dots to its end. E.g., *192.168.1.1* will become *192.168.1.1.....*  Since it is
   pointless to change this parameter, we simply hard code it into relevant codes.

Launch the system
------------------

   1. log into **nerv1** using (remember to replace nerv1's ID with the correct one):
   ~~~
   kubectl exec -it --namespace=fisher-coding nerv1-3622045861-1g0l2 -- bash
   ~~~
   2. Run:
~~~
./Master-Detection uncoded
~~~
for uncoded TeraSort, or 
~~~
./Master-Detection coded
~~~
    for coded TeraSort.

Obtain the result
------------------
The anomaly IPs will be listed in master's **/root/TeraSort/Output/result.txt**
(after running uncoded TeraSort) and **/root/TeraSort/Output/result-C.txt**
(after running coded TeraSort). It looks like:
~~~
192.168.1.1
147.32.84.165
~~~

How do the nodes know each other's address
-------------------------------------------
This is done in **Master-Network.sh** as follows.

Currently, we assue that after the system/deployment is launched, the
**NODE_IPS** variable of the master contains the IP addresses of the workers.
Thus, we read the IP adresses from **NODE_IPS** and create an **ips.txt**,
which looks like:
~~~
10.255.9.140
10.255.9.142
10.255.9.144
~~~

You can also manually create this file and place in master:/root/TeraSort/.

With **ips.txt** available, **Master-Network.sh** will add them to
**/etc/hosts** file of the master with a tag of *n1, n2, n3*, respectively.
At the end of **/etc/hosts** file, you should see something like:
~~~
...

10.255.9.140 n1
10.255.9.142 n2
10.255.9.144 n3
~~~

Then whenever *n1, n2, n3* are quoted in any scripts (Shell, Python, C++ ,,,)
or programs of the master node, they are understood as the assocaited IP.


TODO
====

Fix the conflict between rate-limiting and SSH
-----------------------------------------------
Currently, rate-limiting is not applied: We notice that rate limiting the worker's bandwidth using **tc qdisc** will block SSH access from the master to the workers. Thus, in the current implementation, we banned the **NET_ADMIN** privilige of the nodes in **write_coded_deployment_spec.py** So that the nodes cannot limit the
bandwidth.

Downsize the master image
-------------------------
Due to the use of Hadoop, the size of the master image is too large (over 3GB).
Thus, the master cannot be implemented inside a light-weight cluster. Thus,
we currently launch master in a more capable cluster, and run a fake master
inside the light-weight cluster to remotely control the master using **teraSort.py**.

Current Version
=================
Docker images:

   * fishermanymc/master:v0.03
   * fishermanymc/worker:v0.03

OpenMPI: v1.10.7

Hadoop: v2.8.1

Contact
=======
mingchao.yu@usc.edu

All Rights Reserved

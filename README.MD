Overview
============

This repo is a complete Docker + Kubernetes implementation of coded Tera-Sort based
anomaly detection system. When executed, it extracts the source IPs from the
detection trace, and returns the source IPs that are too active, which are
likely to be anomaly.

Build the Docker Images
==========================

Master
------
Enter the **/master** folder, and run:
~~~
docker build -t master:v0.03 .
docker tag master:v0.03 fishermanymc/master:v0.03
docker push fishermanymc/master:v0.03
~~~

Worker
------
Enter the **/worker** folder, and run:
~~~
docker build -t worker:v0.03 .
docker tag master:v0.03 fishermanymc/worker:v0.03
docker push fishermanymc/worker:v0.03
~~~

Note: if you use other images/tags, don't forget to revise **k8s_coding.py**
in the main folder accordingly.


Launch the Service & Deployment
==================

   1. Log into a Kubernetes cluster;
   2. select 4 nodes;
   3. Place their name (e.g. ubuntu-2gb-nyc2-02) to **nodes_coded.txt**
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

Launch the Detection System
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
   

Parameter setting
------------------
   * anomaly threshold: if the number of times an IP appears is greater than this
     threshold, we will treat this IP as an anomaly. This parameter can be set
     at Master-Dection.sh
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
192.168.1.1.....
147.32.84.165...
~~~


TODO
====

Fix the conflict between rate-limiting and SSH
-----------------------------------------------
Currently, rate-limiting is not applied: We notice that rate limiting the worker's bandwidth using **tc qdisc** will block SSH access from the master to the workers. Thus, in the current implementation, we banned the **NET_ADMIN** privilige of the nodes in **write_coded_deployment_spec.py** So that the nodes cannot limit the
bandwidth.


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

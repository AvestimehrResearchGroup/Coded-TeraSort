import time
import os
from os import path
from multiprocessing import Process
from write_coded_service_specs import *
from write_coded_deployment_specs import *
from kubernetes import client, config
from pprint import *
import os

CODING_NAMESPACE = 'fisher-coding'

def read_node_list(path2):

    nodes = {}
    node_file = open(path2, "r")
    for line in node_file:
        node_line = line.strip().split(" ")
        nodes.setdefault(node_line[0], [])
        for i in range(1, len(node_line)):
            nodes[node_line[0]].append(node_line[i])
    print(nodes)
    return nodes

def launch_services(path='nodes_coded.txt'):
    """
        This loads the task graph and node list
    """
    nodes = read_node_list(path)


    """
        This loads the kubernetes instance configuration.
        In our case this is stored in admin.conf.
        You should set the config file path in the apac_config.py file.
    """    
    config.load_kube_config(config_file = os.environ['KUBECONFIG'])    
    """
        We have defined the namespace for deployments in apac_config
    """
    namespace = CODING_NAMESPACE
    
    """
        Get proper handles or pointers to the k8-python tool to call different functions.
    """
    v1 = client.CoreV1Api()
    k8s_beta = client.ExtensionsV1beta1Api()

    # # get the list of nodes
    # ret = v1.list_node()

    """
        Loop through the list of nodes and run all profiler related k8 deployment, replicaset, pods, and service.
        You can always check if a service/pod/deployment is running after running this script via kubectl command.
        E.g., 
            kubectl get svc -n "namespace name"
            kubectl get deployement -n "namespace name"
            kubectl get replicaset -n "namespace name"
            kubectl get pod -n "namespace name"
    """
    service_ips = ''
    for i in sorted(nodes, key=lambda node: int(node[4:])):
        """
            Generate the yaml description of the required service for each node
            The returned valued "body" will contain all necessary details for the service related to the deployment on a node.
        """
        #break
        serv = write_profiler_service_specs(name = i)
        # Call the Kubernetes API to create the service
        ser_resp = v1.create_namespaced_service(namespace, serv)
        print("Service created. status = '%s'" % str(ser_resp.status))
        if i != 'nerv1':
            service_ips += ser_resp.spec.cluster_ip + ':'
        #print(serv)
    service_ips = service_ips[:-1]
    return service_ips

def launch_deployments(service_ips, path='nodes_coded.txt', masterIP='10.10.10.10'):
    """
        This loads the task graph and node list
    """
    nodes = read_node_list(path)


    """
        This loads the kubernetes instance configuration.
        In our case this is stored in admin.conf.
        You should set the config file path in the apac_config.py file.
    """    
    config.load_kube_config(config_file = os.environ['KUBECONFIG'])    
    """
        We have defined the namespace for deployments in apac_config
    """
    namespace = CODING_NAMESPACE
    
    """
        Get proper handles or pointers to the k8-python tool to call different functions.
    """
    v1 = client.CoreV1Api()
    k8s_beta = client.ExtensionsV1beta1Api()
    num = 1
    #service_ips = masterIP + ':' + service_ips
    for i in sorted(nodes, key=lambda node: int(node[4:])):
        """
            Generate the yaml description of the required deployment for the profiles
        """
        if i == 'nerv1':
            dep = write_profiler_specs(name = i, label = i, host = nodes[i][0], master_or_slave = 'slave', node_ips = str(service_ips), slave_num = str(num), total_slaves = str(len(nodes.keys())), image = 'fishermanymc/master:v0.03')
        else:
            dep = write_profiler_specs(name = i, label = i, host = nodes[i][0], master_or_slave = 'slave', node_ips = str(service_ips), slave_num = str(num), total_slaves = str(len(nodes.keys())), image = 'fishermanymc/worker:v0.03')
        num += 1
        pprint(dep)
        # # Call the Kubernetes API to create the deployment
        resp = k8s_beta.create_namespaced_deployment(body = dep, namespace = namespace)
        print("Deployment created. status ='%s'" % str(resp.status))
        #print(dep)

if __name__ == '__main__':
    service_ips = launch_services()
    launch_deployments(service_ips)

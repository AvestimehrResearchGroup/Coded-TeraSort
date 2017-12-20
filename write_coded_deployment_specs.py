import yaml

template = """
apiVersion: extensions/v1beta1
kind: Deployment
metadata:
  name: {name}
spec:
  template:
    metadata:
      labels:
        app: {label}
    spec:
      nodeSelector:
        kubernetes.io/hostname: {host}
      containers:
      - name: coded-computation
        image: {image} 
        imagePullPolicy: Always 
        ports:
        - containerPort: 57021
        - containerPort: 22
        env:
        - name: APP_PATH 
          value: static
        - name: APP_NAME 
          value: logisticRegression
        - name: LDPC 
          value: 1
        - name: M_OR_S
          value: {master_or_slave}
        - name: NODE_IPS
          value: {node_ips}
        - name: SLAVE_NUM
          value: {slave_num}
        - name: TOTAL_SLAVES
          value: {total_slaves}
        securityContext:
          capabilities:
            add:
             - NET_ADMIN
"""


## \brief this function genetares the service description yaml for a task 
# \param kwargs             list of key value pair. 
# In this case, call argument should be, 
# name = {taskname}, dir = '{}', host = {hostname}

def write_profiler_specs(**kwargs):
    specific_yaml = template.format(**kwargs)
    dep = yaml.load(specific_yaml, Loader=yaml.BaseLoader)
    return dep

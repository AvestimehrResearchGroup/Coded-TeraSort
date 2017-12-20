FROM ubuntu:16.04
# FROM openjdk:9-jre-slim
RUN apt-get update
RUN apt-get install -yqq openssh-client openssh-server bzip2 wget net-tools sshpass screen
RUN echo 'root:zhifeng' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PermitRootLogin yes/PermitRootLogin yes/' /etc/ssh/sshd_config


RUN apt-get install g++ make openmpi-bin libopenmpi-dev -y
RUN apt-get install sudo -y
COPY standby.sh standby.sh
#WORKDIR /root
EXPOSE 22
#RUN mkdir -p .ssh
# EXPOSE MPI port
#CMD ["python3 ~/standby.py"]
CMD ["bash", "./standby.sh"]

FROM ubuntu:16.04
RUN apt-get update
RUN apt-get install -yqq openssh-client openssh-server bzip2 wget net-tools sshpass screen
RUN echo 'root:zhifeng' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PermitRootLogin yes/PermitRootLogin yes/' /etc/ssh/sshd_config


RUN apt-get install g++ make openmpi-bin libopenmpi-dev -y
RUN apt-get install sudo -y
RUN apt-get install iproute2 -y
RUN mkdir -p /root/TeraSort/Output
COPY rate-limit-then-standby.sh /root/rate-limit-then-standby.sh
EXPOSE 22
CMD ["bash", "/root/rate-limit-then-standby.sh"]

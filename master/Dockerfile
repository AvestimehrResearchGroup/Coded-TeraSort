FROM ubuntu:16.04
RUN apt-get update && apt-get install -y --no-install-recommends apt-utils
RUN apt-get install sudo -y
RUN apt-get install wget -y
RUN wget http://supergsego.com/apache/hadoop/common/hadoop-2.8.1/hadoop-2.8.1.tar.gz -P ~/
RUN tar -zxvf ~/hadoop-2.8.1.tar.gz -C ~/
RUN apt-get install -yqq openssh-client openssh-server bzip2 wget net-tools sshpass screen
RUN apt-get install g++ make openmpi-bin libopenmpi-dev -y

RUN mkdir TeraSort
COPY . ./root/TeraSort
WORKDIR /root/TeraSort
RUN chmod +x Master-Detection.sh
RUN chmod +x Master-PrepareInput.sh
RUN g++ -std=c++11 extractIPs.cpp -o extractIPs
RUN make
RUN g++ -std=c++11 filterResult.cpp -o filterResult
RUN echo 'root:zhifeng' | chpasswd
EXPOSE 22

CMD ["bash", "./standby.sh"]

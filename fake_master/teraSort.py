import os
import time
import subprocess
import pipes


def task(onefile, pathin, pathout):
    realMasterIP = "192.168.0.0"
    outName = "a1"
    # send the input file to the real master
    os.system("sshpass -p 'zhifeng' scp -o StrictHostKeyChecking=no " +
              pathin + onefile + "root@" +
              realMasterIP + ":/root/TeraSort/Input/data.txt")

    # execute uncoded TeraSort
    os.system("sshpass -p 'zhifeng' ssh " + realMasterIP +
              " '/root/TeraSort/Master-Detection.sh uncoded'")

    # wait until the result is avaiable
    while not exists_remote(realMasterIP, "/root/TeraSort/Output/result.txt"):
        time.sleep(1)

    # # execute coded TeraSort
    # os.system("sshpass -p 'zhifeng' ssh " + realMasterIP +
    #           "bash /root/Terasort/Master-Detecion.sh coded")

    # # wait until the result is avaiable
    # while not exists_remote("root@" + realMasterIP,
    #                         "/root/TeraSort/Output/result-C.txt"):
    #     time.sleep(1)

    # download the results
    os.system("sshpass -p 'zhifeng' scp -o StrictHostKeyChecking=no root@" +
              realMasterIP + " /root/TeraSort/Output/result.txt" + ' ' +
              pathout + outName + ".txt")
    # os.system("sshpass -p 'zhifeng' scp root@" + realMasterIP +
    #           "/root/teraSort/Output/result-C.txt" +
    #           pathout + outName + "-C.txt")

    # remove the results from real master
    os.system("sshpass -p 'zhifeng' ssh " + realMasterIP +
              "rm /root/Terasort/Output/result.txt")
    # os.system("sshpass -p 'zhifeng' ssh " + realMasterIP +
    #           "rm /root/Terasort/Output/result.txt")


def exists_remote(host, path):
    """Test if a file exists at path on a host accessible with SSH."""
    status = subprocess.call(
        ['sshpass', '-p', 'zhifeng',
         'ssh', host, 'test -f {}'.format(pipes.quote(path))])
    if status == 0:
        return True
    if status == 1:
        return False


if __name__ == '__main__':
    oneName = 'data.txt'
    pathin = './Input'
    pathout = './Output'
    task(oneName, pathin, pathout)

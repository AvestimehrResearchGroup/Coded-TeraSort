import os
import time


def task(onefile, pathin, pathout):
    realMasterIP = "10.244.9.145"
    outName = "a1"
    # send the input file to the real master
    print("Send file")
    os.system("sshpass -p 'zhifeng' scp -o StrictHostKeyChecking=no " +
              pathin + onefile + " root@" +
              realMasterIP + ":/root/TeraSort/Input/data.txt")

    # Execute uncoded TeraSort
    # For coded TeraSort, replace "uncoded" below by "coded"
    print("Run code")
    os.system("sshpass -p 'zhifeng' ssh " + realMasterIP +
              " '/root/TeraSort/Master-Detection.sh uncoded'")

    # Download the results
    # For coded TeraSort, replace "result.txt" below by "result-C.txt"
    print("Download the result")
    while True:
        try:
            print("Downlao file")
            os.system("sshpass -p 'zhifeng' scp -o" +
                      " StrictHostKeyChecking=no root@" +
                      realMasterIP +
                      ":/root/TeraSort/Output/result.txt " +
                      pathout + outName + ".txt")
            break
        except Exception as e:
            print("waiting for output")
            time.sleep(1)

    # Remove the results from real master
    # For coded TeraSort, replace "result.txt" below by "result-C.txt"
    print("Delete rermote files")
    os.system("sshpass -p 'zhifeng' ssh -o StrictHostKeyChecking=no " +
              realMasterIP +
              " 'rm /root/TeraSort/Output/result.txt'")
    print("Finishsed")


if __name__ == '__main__':
    oneName = 'data.txt'
    pathin = './'
    pathout = './'
    task(oneName, pathin, pathout)

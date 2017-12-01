#include <stdio.h>
#include <fstream>
using namespace std;

void addWorkerIP(char * IPFileName, char * hostFileName){
	ifstream ipFile;
	ipFile.open(IPFileName);
	string myLine;
	std::ofstream hostFile(hostFileName, ios::app);
	hostFile << endl;
	int numWorkers = 1;
	while (!ipFile.eof()){
		if (getline(ipFile, myLine)){
			hostFile << myLine << " n" << numWorkers << endl;
			numWorkers ++;
		}
	}
}

int main(int argc, char *argv[]){
	addWorkerIP(argv[1], argv[2]);
	printf("Worker IPs added");
}
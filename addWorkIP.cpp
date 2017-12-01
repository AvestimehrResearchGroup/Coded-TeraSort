#include <stdio.h>

void addWorkerIP(char * IPFileName){
	printf("%s\n", IPFileName);
}

int main(int argc, char *argv[]){
	addWorkerIP(argv[1]);
	printf("Worker IPs added");
}
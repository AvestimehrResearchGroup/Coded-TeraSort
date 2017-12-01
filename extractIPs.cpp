#include "stdio.h"  // printf
#include <iostream>  // cout
#include <iomanip>  // exit()
#include <fstream>  // file handling
#include <string> // string
#include <vector>  // vector<>
using namespace std;


vector<string> handelLine(string myLine){
    // This function takes a string as the input.
    // The string may contain spaces (i.e., ' ').
    // The function segments the string into several smalle strings
    // based on the spaces.
    // The output is a vector of strings.
    vector<string> sample(1);  // creat a vector of strings, len = 1
    int numSeg = 0;
    int idx = 0;
    for (int i=0; i < myLine.size(); i++){
        if (myLine[i] != ' '){  // the current segment is not completed
            sample[numSeg] += myLine[i];  // add the char to the string
        }
        else{
            numSeg ++;  // current segment completed, move to the next segment
            sample.resize(numSeg + 1);  // increase the vector size by 1
        }
    }
    return sample;
}

vector<vector<string>> handleData(string fileName){
    // This function takes the name of a file (e.g., 'data.txt') as the input.
    // It reads the file line by line.
    // Each line is a string with spaces (i.e., ' ').
    // The function segments each line into several strings based on the spaces. 
    // The output is a 2D matrix. Each row carries the segmented strings
    // of one line.
    ifstream myFile;  // create an object to read the data txt.
    myFile.open(fileName);
    if (!myFile){
        cout << "Unable to open file" << endl;
        exit(1);
    }
    string myLine;  // create an object to read one line of the data
    int numLine = 0;
    // create a 2D matrix with strings as entries
    // each row of this matrix contains all the segments of a line
    vector<vector<string>> samples;
    while (!myFile.eof()){  // while not reaching the end of the file
        if (getline(myFile, myLine)){  // try read a line
            auto sample = handelLine(myLine);
            if (sample.size() > 1){  // skip empty and short lines
                samples.resize(numLine + 1);
                samples[numLine] = sample;
                numLine ++;
            }
        }
    }
    return samples;
}


void extractIPs(string inFileName, string outFileName){
    // each line of the input file has the format of:
    // sth srcIP sth sth ...
    // i.e., the second portion is the IP we want to extract 
    vector<vector<string>> samples = handleData(inFileName);
    ofstream myFile (outFileName);
    string value (10, '*'); // the value of the key, set as 10 "*"s.
    int numSamples = samples.size();
    int maxSize = 0;
    for (int i = 0; i < numSamples; i++){  // Find the largest IP length
        if (samples[i][1].size() > maxSize){
            maxSize = samples[i][1].size();
        }
    }
    for (int i = 0; i < samples.size(); i ++){
        myFile << samples[i][1];  // the srcIP
        int diff = maxSize - samples[i % samples.size()][1].size();
        if (diff > 0){
            for (int j = 0; j < diff; j++){
                myFile << ".";
            }
        }
        myFile << value;
    }
}


int main(int argc, char *argv[]){
    extractIPs(argv[1], "./Input/srcIPs.txt");
}
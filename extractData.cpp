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


void extracIPs(string inFileName, string outFileName){
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


void countFrequency(vector<unsigned char *> locaList, string outFileName){
    // initialize
    int lenIP = 13;
    vector<string> IPs(1);
    vector<int> counts(1);
    IPs[0].resize(lenIP);
    copy(locaList[0], locaList[0] + lenIP, IPs[0].begin());
    int numIPs = 0;  // the actual number of different IPs - 1

    // count frequency
    for (unsigned int i = 0; i < locaList.size(); i++){
            string ip(locaList[i], locaList[i] + lenIP);
            if (ip.compare(IPs[numIPs]) != 0){  // find a new IP
                numIPs ++;
                IPs.resize(numIPs + 1);
                IPs[numIPs].resize(lenIP);
                counts.resize(numIPs + 1);
                std:copy(locaList[i], locaList[i] + lenIP, IPs[numIPs].begin());
            }
            counts[numIPs] ++;
    }
    // write into a file
    ofstream myFile (outFileName);
    for (unsigned int i = 0; i < IPs.size(); i ++){
        myFile << IPs[i] << ' ' << counts[i] << endl;
    }
}


// C++ program for implementation of Heap Sort
// The sort is in descending order.
// Reference:
// https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm 

void heapify(vector<int> &vec, vector<int> &idx, int n, int i)
{
    int smallest = i;  // Initialize largest as root
    int l = 2 * i + 1;  // left = 2*i + 1
    int r = 2 * i + 2;  // right = 2*i + 2
 
    // If left child is larger than root
    if (l < n && vec[l] < vec[smallest])
        smallest = l;
 
    // If right child is larger than largest so far
    if (r < n && vec[r] < vec[smallest])
        smallest = r;
 
    // If largest is not root
    if (smallest != i)
    { 
        swap(vec[i], vec[smallest]);
        swap(idx[i], idx[smallest]);
 
        // Recursively heapify the affected sub-tree
        heapify(vec, idx, n, smallest);
    }
}
 
// main function to do heap sort
// It will return the order of the sorting
// E.g., if vec = [12, 56, 23], then return idx = [1, 2, 0]
vector<int> heapSort(vector<int> &vec)
{   
    int n = int(vec.size());
    vector<int> idx(vec.size());
    for (int i = 0; i < n; i++)
        idx[i] = i;
    // Build heap (revecange vector)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(vec, idx, n, i);
    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--)
    {
        // Move current root to end
        swap(vec[0], vec[i]);
        swap(idx[0], idx[i]);
        heapify(vec, idx, i, 0);
    }
    return idx;
}


void sortFrequency(vector<string> inFileNames, string outFileName){
    vector<string> IPs;
    vector<int> counts;
    int numIPs = 0;
    // read files
    for (int i = 0; i < int(inFileNames.size()); i ++){
        ifstream myFile;  // create an object to read the data txt.
        myFile.open(inFileNames[i]);
        string myLine;
        while (!myFile.eof()){  // while not reaching the end of the file
            if (getline(myFile, myLine)){  // try read a line
                IPs.resize(numIPs + 1);
                unsigned int loc = 0;
                // read IP
                while (myLine[loc] != ' ' && loc < myLine.size()){
                    IPs[numIPs] += myLine[loc];
                    loc ++;
                }
                // read the count
                string freq;
                while (loc < myLine.size()){
                    freq += myLine[loc];
                    loc ++;
                }
                counts.resize(numIPs + 1);
                counts[numIPs] = atoi(freq.c_str());
                numIPs ++;
            }
        }
    }
    // sort the counts
    auto idx = heapSort(counts);  // sort counts, and return idx
    ofstream myFile (outFileName);
    for (int i = 0; i < numIPs; i++){
        myFile << IPs[idx[i]] << ' ' << counts[i] << endl;
    }
}

int main(){
    // Example of extract IPs and write into IP-key pairs.
    string inFileName = "./Input/data.txt";
    string outFileNameIP = "./Input/srcIPs.txt";
    extracIPs(inFileName, outFileNameIP);

    // Example of count frequency
    // prepare input
    int lenPair = 23;
    unsigned char lList[70] = "192.168.1.1..**********192.168.1.2..**********192.168.1.2..**********";
    vector<unsigned char *> locaList(3);
    locaList[0] = lList;  // the starting point of the 1st pair
    locaList[1] = lList + lenPair;  // the starting point of the 2nd pair
    locaList[2] = lList + lenPair * 2;  // the starting point of the 3rd pair
    // count frequency and output a file
    string outFileNameFreq = "./Output/countIPs.txt";
    countFrequency(locaList, outFileNameFreq);

    // Example of merging the workers feedback and sort the final frequency
    vector<string> inFileNames(1);
    inFileNames[0] = "./Output/countIPs.txt";
    string outFileName = "./Output/sortedIPs.txt";
    sortFrequency(inFileNames, outFileName);
}


// to make:
// g++ -std=c++11 extractData.cpp -o test

// to run:
// ./test
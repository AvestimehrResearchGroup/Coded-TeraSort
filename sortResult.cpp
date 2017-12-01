#include "stdio.h"  // printf
#include <iostream>  // cout
#include <iomanip>  // exit()
#include <fstream>  // file handling
#include <string> // string
#include <vector>  // vector<>
using namespace std;


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


void sortResult(string inFileName, string outFileName){
    vector<string> IPs;
    vector<int> counts;
    int numIPs = 0;
    // read files
    ifstream myFile;  // create an object to read the data txt.
    myFile.open(inFileName);
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
    // sort the counts
    auto idx = heapSort(counts);  // sort counts, and return idx
    ofstream outFile (outFileName);
    for (int i = 0; i < numIPs; i++){
        outFile << IPs[idx[i]] << ' ' << counts[i] << endl;
    }
}

int main(int argc, char *argv[]){
    sortResult(argv[1], "./Output/result.txt");
}


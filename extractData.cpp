#include "stdio.h"  // printf
#include <iostream>  // std::cout
#include <iomanip>  // exit()
#include <fstream>  // file handling
#include <string> // std::string
#include <vector>  // std::vector<>


std::vector<std::string> handelLine(std::string myLine){
    // This function takes a string as the input.
    // The string may contain spaces (i.e., ' ').
    // The function segments the string into several smalle strings
    // based on the spaces.
    // The output is a vector of strings.
    std::vector<std::string> sample(1);  // creat a vector of strings, len = 1
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

std::vector<std::vector<std::string>> handleData(std::string fileName){
    // This function takes the name of a file (e.g., 'data.txt') as the input.
    // It reads the file line by line.
    // Each line is a string with spaces (i.e., ' ').
    // The function segments each line into several strings based on the spaces. 
    // The output is a 2D matrix. Each row carries the segmented strings
    // of one line.
    std::ifstream myFile;  // create an object to read the data txt.
    myFile.open(fileName);
    if (!myFile){
        std::cout << "Unable to open file" << std::endl;
        exit(1);
    }
    std::string myLine;  // create an object to read one line of the data
    int numLine = 0;
    // create a 2D matrix with strings as entries
    // each row of this matrix contains all the segments of a line
    std::vector<std::vector<std::string>> samples;
    while (!myFile.eof()){  // while not reaching the end of the file
        if (std::getline(myFile, myLine)){  // try read a line
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

int main(){
    std::string fileName = "data.txt";
    std::ofstream myFile ("srcIPs.txt");
    std::string value (10, '*'); // the value of the key, set as 10 "*"s.
    std::vector<std::vector<std::string>> samples = handleData(fileName);
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
        // myFile << std::endl;
    }
}


// to make:
// g++ -std=c++11 extractData.cpp -o test

// to run:
// ./test
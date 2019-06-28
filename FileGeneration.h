#ifndef FILEGENERATION_H
#define FILEGENERATION_H

#include <iostream>
#include <fstream>

#define MAX_PROCESSES 10
using namespace std;

class FileGeneration{
public:
    FileGeneration();
    ~FileGeneration();

    //Generate file with a random number of processes 
    void generateInputFile(string fileName, bool isDistributed);
    //Generate file with a specific number of processes
    void generateInputFile(string fileName, int totalProc, bool isDistributed);

private:
    
    ofstream outputFile;

    string input, fileName;
    int totalProc, procSwitch;
    int  procNum = 1, arrivalTime = 0, burstTime, relativeDealine, period;

    
    void setFileName(string fileName);
    void generateFile(bool isDistributed);
    void setTotalProc(int totalProc);
    void randomizeDist();
    void randomizeNotDist();
    
    bool fileExists(const string& name);

};

#endif
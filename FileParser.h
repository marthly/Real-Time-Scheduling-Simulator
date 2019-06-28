#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "Command.h"
#include "Process.h"
#include <iostream>
using namespace std;

class FileParser{
public:
    FileParser();
    ~FileParser();

    Process parseFile(string inputFile);
    bool fileOpen();
       

private:

    Process process;

    string fileName;

    int totalProc, procSwitch;
    int procNum, arrivalTime, burstTime, relativeDeadline, period, iterator = 1 ;

    bool openFile;
};

#endif
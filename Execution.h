#ifndef EXECUTION_H
#define EXECUTION_H

#include "Process.h"
#include "Command.h"
#include "FileParser.h"

#include <vector>
#include <iostream>
using namespace std;

class Execution{

public:
    Execution();
    ~Execution();

    void execute(string inputCommand);

private:

    Process process;
    Command command;
    FileParser fileParser;   

    vector<int> vectorCPUBound;

    ofstream resultsFile;

    int startTime, currentTime = 0, remainingDeadline, remainingExecutionTime, partiallyExecutedTime, requiredCPUTime;
    int nextAvailableProc, nextAvailableProcTime = 0, nextAvialableProcBurstTime, nextAvailableProcDeadline = 0; 
    int currentRunningProc, currentRunningProcDeadline, currentRunningProcPosition, currentRunningProcBurstTime;
    
    float nextAvailableProcPriority, currentRunningProcPriority;

    bool preEmpted = false, ganttPass = true, missingProcDuplicate = false;

    //Execute Rate Monotonic Algorithm
    void rateMonotonic(Process process);
    void rateMonotonicAlg(Process process);
    //Execute Earliest Deadline First Algorithm
    void earlyDeadline(Process process);
    void earlyDeadlineAlg(Process process);
    //Execute Deadline Monotonic Algorithm
    void deadlineMono(Process process);
    void deadlineMonoAlg(Process process);

    void minimalView(Process process);
    void verboseView(Process process);
    void detailedView(Process process);
    void initialiseResultsFile();

    void checkIdle();

    void report(Process process);
    void resetVariable();
    void removeFromCPUBound(int procPos);
};

#endif
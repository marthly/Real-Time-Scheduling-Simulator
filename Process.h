#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Process{
public:
    Process();
    ~Process();
    
    void setTotalProcess(int totalProc);
    void setProcessSwitch(int procSwitch);
    void setProcessNum(int procNum);
    void setArrivalTime(int arrivalTime);
    void setBurstTime(int burstTime);
    void setRelativeDeadLine(int relativeDeadline);
    void setPeriod(int period);
    void setPriority_RM(int period);
    void setPriority_DM(int relativeDeadline);
    
    void setFinishTime(int process, int finishTime);
    void setRemainingDeadline(int process, int remainingDeadline);
    void setProcessDeadlineTime();
    void setProcessAvailabilityTime();
    void setRemainingExecutionTime(int process, int remainingExecutionTime);
    
    void addVectorProcNum(int period);
    void addVectorPeriod(int period);
    void addVectorArrivalTime(int arrivalTime);
    void addVectorBurstTime(int burstTime);
    void addVectorRelativeDeadline(int relativeDeadline);
    void addVectorProcess(Process process);
    void addVectorMissedProcess(int process);
    void addVectorMissedProcessFinishTime(int finishTime);
    void addVectorMissedProcessExpectedFinishTime(int expectedTime);

    void removeAvailableTime(int process);
    void removeDeadlineTime(int process);
    void initialiseFinishTime();
    
    int getTotalProcess();
    int getProcessSwitch();
    int getArrivalTime(int process);
    int getBurstTime(int process);
    int getRelativeDeadline(int process);
    int getPeriod(int process);
    int getTotalScheduleTime();
    int getProcessAvailabilitySize(int process);
    int getRemainingExecutionTime(int process);
    int getFinishTime(int process);
    int getFinishTimeSize(int process);    
    int getRemainingDeadline(int process);
    int getLCM_LIMIT_VALUE();
    unsigned long int getLCM_Value();
    
    float getSystemUtilization();
    float getPriority_RM(int process);
    float getPriority_DM(int process);

   
    
    //Return a vector with all burst time
    vector<int> getVectorBurstTime();
    //Returns a vector with all the periods
    vector<int> getVectorPeriod();
    //Return a vector with missed processes
    vector<int> getVectorMissedProcess();
    //Return a vector with missed process times
    vector<int> getVectorMissedProcessFinishTime();
    vector<int> getVectorMissedProcessExpectedFinishTime();
    //Return a queue with Deadline Time of processes
    queue<int> getProcessDeadlineTime(int process);
     //Return a queue with Availability Time of processes
    queue<int> getProcessAvailabilityTime(int process);


    //Return bool value of Sufficient Condition for Rate Monotonic
    bool isSuffCond_RM();
    //Return bool value of Necessary Condition for Rate Monotonic
    bool isNecCond_RM();
    
    //Return bool value of Sufficient Condition for Earliest Deadline First
    bool isSuffCond_EDF();
    //Return bool value of Necessary Condition for Earliest Deadline First
    bool isNecCond_EDF();

    //Return bool value of Sufficient Condition for Deadline Monotonic
    bool isSuffCond_DM();
    //Return bool value of Necessary Condition for Deadline Monotonic
    bool isNecCond_DM();

    bool isLCM_LIMIT_PASSED();

private:
    #define LCM_LIMIT 100
    
    //Stores a list of all processes created 
    vector<Process> vectorProcess;
    //Return a list with Priority that is order by its priority
    vector<int> vectorOrderedProcByPriority;
    //Store a list of the finish time of the processes
    queue<int> queueFinishTimeList;
    //Stores a list of a process availability time
    queue<int> queueAvailabilityTime;
    //Stores a list of a process deadline time
    queue<int> queueDeadlineTime;
    //Stores content for each process
    vector<queue<int>> queueFinishTime, queueProcessAvailabilityTime, queueProcessDeadlineTime;
    vector<int> vectorProcNum, vectorArrivalTime, vectorBurstTime, vectorRelativeDeadline, vectorPeriod;
    vector<int> vectorMissedProcess, vectorMissedProcessFinishTime, vectorMissedProcessExpectedFinishTime;  

    //GCD - numerator,denominator,quotient,remainder,previous remainder.
    int num,denom,quo,rem=1,preRem; 
    unsigned long int LCM_Value;

    int totalProc, procSwitch, procNum, arrivalTime, burstTime, relativeDeadline, period; 
    int totalScheduleTime, remainingDeadline, remainingExecutionTime, finishTime;
    float priority_RM, priority_DM, systemUtilization = 0;

    bool suffCond_RM, necCond_RM, suffCond_EDF, necCond_EDF, suffCond_DM, necCond_DM, LCM_LIMIT_PASSED = false;

    int MAX(vector<int> vectorArrivalTime);
    //Return int value of Lowest Common Multiple
    int LCM(vector<int> vectorPeriod);
    // Return int value of Greatest Common Denominator (Eucledian Method)
    unsigned long int GCD(unsigned long int x, unsigned long int y);
    // Reset the variables of GCD
    void reset();

    float I(vector<int> vectorOrderedProcByPriority, float procNum, int iterator);
    void quickSort(int deadline[],int process[], int left, int right);
};

#endif
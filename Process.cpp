#include "Process.h"
#include <tgmath.h>
#include <iostream>

Process::Process(){
}

Process::~Process(){
}

void Process::setTotalProcess(int totalProc){
    this->totalProc = totalProc;
}

void Process::setProcessSwitch(int procSwitch){
    this->procSwitch = procSwitch;
}

void Process::setProcessNum(int procNum){
    this->procNum = procNum;
}

void Process::setArrivalTime(int arrivalTime){
    this->arrivalTime = arrivalTime;
}

void Process::setBurstTime(int burstTime){
    this->burstTime = burstTime;
}

void Process::setRelativeDeadLine(int relativeDeadline){
    this->relativeDeadline = relativeDeadline;
}

void Process::setPeriod(int period){
    this->period = period;
}

void Process::setPriority_RM(int period){
    priority_RM = 1.0/period;
}

void Process::setPriority_DM(int relativeDeadline){
    priority_DM = 1.0/relativeDeadline;
}


//==========================================================================================
void Process::setProcessAvailabilityTime(){
    int iterator = 0, availableTime = 0;
    totalScheduleTime = getTotalScheduleTime();

    //cout << "Total Schedule Time: " << totalScheduleTime << endl;

    for(procNum = 1; procNum <= totalProc; procNum++){
        arrivalTime = vectorProcess[procNum - 1].arrivalTime;
        period = vectorProcess[procNum - 1].period;       

        availableTime = arrivalTime + (iterator * period);

        while(availableTime <= totalScheduleTime){
            queueAvailabilityTime.push(availableTime);
            //cout << "Available Time: " << availableTime << endl;
            iterator++;
            availableTime = arrivalTime + (iterator * period);
        }        
        //cout << endl;

        queueProcessAvailabilityTime.push_back(queueAvailabilityTime);
        queueAvailabilityTime;
        while(iterator > 0){
            queueAvailabilityTime.pop();
            iterator--;
        }
    }    
}

void Process::setProcessDeadlineTime(){
    int deadlineTime;
    totalScheduleTime = getTotalScheduleTime();

    //cout << "Total Schedule Time: " << totalScheduleTime << endl;

    for(procNum = 1; procNum <= totalProc; procNum++){
        arrivalTime = vectorProcess[procNum - 1].arrivalTime;
        deadlineTime = vectorProcess[procNum - 1].relativeDeadline;
        period = vectorProcess[procNum - 1].period;
        
        deadlineTime = arrivalTime + deadlineTime;
        setRemainingDeadline(procNum,deadlineTime);

        while(queueDeadlineTime.size() < queueProcessAvailabilityTime[procNum - 1].size()){
            //cout << "Deadline Time: " << deadlineTime << endl;
            queueDeadlineTime.push(deadlineTime);
            deadlineTime += period;
        }
        //cout << endl;
        
        queueProcessDeadlineTime.push_back(queueDeadlineTime);
        
        while(queueDeadlineTime.size() > 0){
            queueDeadlineTime.pop();
        }
    }
    //exit(0);
}

void Process::setRemainingExecutionTime(int process, int remainingExecutionTime){
    vectorProcess[process - 1].remainingExecutionTime = remainingExecutionTime;
}

void Process::setFinishTime(int process, int finishTime){
    queueFinishTime[process - 1].push(finishTime);
}

void Process::setRemainingDeadline(int process, int remainingDeadline){
    vectorProcess[process - 1].remainingDeadline = remainingDeadline;
}

//==========================================================================================
void Process::addVectorProcNum(int procNum){
    vectorProcNum.push_back(procNum);
}

void Process::addVectorPeriod(int period){
    vectorPeriod.push_back(period);                        
} 

void Process::addVectorArrivalTime(int arrivalTime){
    vectorArrivalTime.push_back(arrivalTime);
}

void Process::addVectorBurstTime(int burstTime){
    vectorBurstTime.push_back(burstTime);
}

void Process::addVectorRelativeDeadline(int relativeDeadline){
    vectorRelativeDeadline.push_back(relativeDeadline);
}

void Process::addVectorProcess(Process process){
    vectorProcess.push_back(process);
} 

void Process::addVectorMissedProcess(int process){
    vectorMissedProcess.push_back(process);
}

void Process::addVectorMissedProcessFinishTime(int finishTime){
    vectorMissedProcessFinishTime.push_back(finishTime);
}

void Process::addVectorMissedProcessExpectedFinishTime(int expectedTime){
    vectorMissedProcessExpectedFinishTime.push_back(expectedTime);
}

//==========================================================================================
void Process::removeAvailableTime(int process){
    queueProcessAvailabilityTime[process - 1].pop();
}

void Process::removeDeadlineTime(int process){
    queueProcessDeadlineTime[process - 1].pop();
}

void Process::initialiseFinishTime(){
    for(int i = 0; i < totalProc; i++){
        queueFinishTime.push_back(queueFinishTimeList);
    }

}

//==========================================================================================
int Process::getTotalProcess(){                                                                                      
    return totalProc;                                                                                                                   
}

int Process::getProcessSwitch(){
    return procSwitch;
}

int Process::getArrivalTime(int process){
    return vectorProcess[process - 1].arrivalTime;
}

int Process::getBurstTime(int process){
    return vectorProcess[process - 1].burstTime;
}

int Process::getRelativeDeadline(int process){
    return vectorProcess[process - 1].relativeDeadline;
}

int Process::getPeriod(int process){
    return vectorProcess[process - 1].period;
}

int Process::getTotalScheduleTime(){
    bool isSame;
    for(int i = 0; i < totalProc; i++){
        if(vectorArrivalTime[0] == vectorArrivalTime[i]){
            isSame = true;
        }
        else{
            isSame = false;
            i = totalProc;
        } 
          
    }
    if(isSame){
        totalScheduleTime = vectorArrivalTime[0] + LCM(vectorPeriod);
    }
    else{
        totalScheduleTime = MAX(vectorArrivalTime) + (2 * LCM(vectorPeriod));
    }
    
    return totalScheduleTime;
}

int Process::getProcessAvailabilitySize(int process){
    return queueProcessAvailabilityTime[process - 1].size();
}

int Process::getRemainingExecutionTime(int process){
    return vectorProcess[process - 1].remainingExecutionTime;
}

int Process::getFinishTimeSize(int process){
     return queueFinishTime[process - 1].size();
}

int Process::getFinishTime(int process){
   int value = queueFinishTime[process - 1].front();
   queueFinishTime[process - 1].pop();
   return value;
}

int Process::getRemainingDeadline(int process){
    return vectorProcess[process - 1].remainingDeadline;
}

unsigned long int Process::getLCM_Value(){
    return LCM_Value;
}

int Process::getLCM_LIMIT_VALUE(){
    return LCM_LIMIT;
}

//==========================================================================================
float Process::getSystemUtilization(){
    systemUtilization = 0;
    for(int i = 0;i < totalProc; i++){
        systemUtilization += static_cast<float>(vectorBurstTime[i])/vectorPeriod[i];;
    }
    return systemUtilization;
}

float Process::getPriority_RM(int process){
    return vectorProcess[process - 1].priority_RM;
}

float Process::getPriority_DM(int process){
    return vectorProcess[process - 1].priority_DM;
}

//==========================================================================================
bool Process::isLCM_LIMIT_PASSED(){
    return LCM_LIMIT_PASSED;
}

//==========================================================================================

queue<int> Process::getProcessAvailabilityTime(int process){
    return queueProcessAvailabilityTime[process - 1];
}

queue<int>  Process::getProcessDeadlineTime(int process){
    return queueProcessDeadlineTime[process - 1];
}

vector<int> Process::getVectorBurstTime(){
    return vectorBurstTime;
}

vector<int> Process::getVectorPeriod(){
    return vectorPeriod;
}

vector<int> Process::getVectorMissedProcess(){
    return vectorMissedProcess;
}

vector<int> Process::getVectorMissedProcessFinishTime(){
    return vectorMissedProcessFinishTime;
}

vector<int> Process::getVectorMissedProcessExpectedFinishTime(){
    return vectorMissedProcessExpectedFinishTime;
}

//==========================================================================================
bool Process::isSuffCond_RM(){
    float total, utilization = getSystemUtilization();
    total = totalProc * (pow(2,(1/static_cast<float>(totalProc))) - 1);
    suffCond_RM = (utilization < total) ? true:false;
    return suffCond_RM;
}

bool Process::isNecCond_RM(){
    float utilization = getSystemUtilization();

    necCond_RM = (utilization < 1) ? true:false;
    return necCond_RM;
}

bool Process::isSuffCond_EDF(){
	float utilization;

    for(int i = 0;i < totalProc; i++){
        utilization += static_cast<float>(vectorBurstTime[i])/vectorRelativeDeadline[i];;
    }

    suffCond_EDF = (utilization < 1) ? true:false;

    return suffCond_EDF;
}

bool Process::isNecCond_EDF(){
    float utilization = getSystemUtilization();

    necCond_EDF = (utilization < 1) ? true:false;
    
    return necCond_EDF;
}

bool Process::isSuffCond_DM(){
    float value;
    int iterator = 1;
    int deadline[totalProc], process[totalProc];
    
    for(int i = 0; i < totalProc; i++){
        process[i] = i + 1;
    }    
    
    for(int i = 0; i < totalProc; i++){
        deadline[i] = getRelativeDeadline(i+1);
    }    
    
    quickSort(deadline,process,0,totalProc-1);
    
    for(int i = 0;i < totalProc;i++){
        vectorOrderedProcByPriority.push_back(process[i]);
    }


    while(vectorOrderedProcByPriority.size() > 0){
        procNum = vectorOrderedProcByPriority[iterator - 1];
        burstTime = getBurstTime(procNum);
        relativeDeadline = getRelativeDeadline(procNum);
        period = getPeriod(procNum);

        value = (static_cast<float>(burstTime)/relativeDeadline) + (static_cast<float>(I(vectorOrderedProcByPriority, procNum, iterator))/relativeDeadline);
        //cout << burstTime << "/" << relativeDeadline << " + " << I(vectorOrderedProcByPriority, procNum, iterator) << "/" << relativeDeadline << " = " << value << endl << endl;

        if(value > 1){
            return false;
        }
        else if(iterator == vectorOrderedProcByPriority.size()){
            return true;
        }
        iterator++;
    }
}

bool Process::isNecCond_DM(){
    float utilization = getSystemUtilization();

    necCond_EDF = (utilization <= 1) ? true:false;
    
    return necCond_EDF;
}

//==========================================================================================
int Process::MAX(vector<int> vectorArrivalTime){
    int x, y, i = 1, result;
    x = vectorArrivalTime[0];
    y = vectorArrivalTime[1];

    while(i <= totalProc){
        result = (x > y) ? x:y;
        x = result;
        y = vectorArrivalTime[i++];
    }
    return result;
}

int Process::LCM(vector<int> vectorPeriod){
    unsigned long int x, y , i = 1, result;
    x = vectorPeriod[0];
    y = vectorPeriod[1];
    
    while(i <= totalProc){
        result = (x*y)/GCD(x,y);        
        x = result;
        y = vectorPeriod[i++];
        reset();
    }

    if(result > LCM_LIMIT){
        LCM_Value = result;
        result = LCM_LIMIT; 
        LCM_LIMIT_PASSED = true;
    }
    return result;
}

unsigned long int Process::GCD(unsigned long int x, unsigned long int y){
	num = (x > y) ? x:y;
	denom = (y < x) ? y:x;
    
    if(denom == 0){
        return num;
    }
    else if(rem <= 0){
        return preRem;
    }
    else{
        preRem = rem;
        rem = num % denom;
        quo = num / denom;
        num = denom;
        denom = rem;
    }
    return GCD(num,denom);		
}

void Process::reset(){
	num,denom=1,quo,rem=1,preRem;
}

float Process::I(vector<int> vectorOrderedProcByPriority, float procNum, int iterator){
    float value = 0.0;
    float burstTimeValue, relativeDeadlineValue, periodValue;

    if(iterator == 1){
        value = 0.0;
    }
    else{

        for(int i = 1; i < iterator;i++){

            burstTimeValue = getBurstTime(vectorOrderedProcByPriority[i-1]);
            relativeDeadlineValue = getRelativeDeadline(procNum);
            periodValue = getPeriod(vectorOrderedProcByPriority[i-1]);
            //cout << relativeDeadlineValue << "/" << periodValue << "*" << burstTimeValue << endl << endl;
            value += ceil(static_cast<float>(relativeDeadlineValue)/periodValue) * burstTimeValue;
        }
    }
    return value;
}

void Process::quickSort(int deadline[],int process[], int left, int right){
    int  i = left, j = right, pivot = deadline[(left + right) / 2], temp;
    
    while(i <= j){

        while(deadline[i] < pivot){
            i++;
        }
        while(deadline[j] > pivot){
            j--;
        }     

        if(i<=j){
            temp = deadline[i];
            deadline[i] = deadline[j];
            deadline[j] = temp;

            temp = process[i];
            process[i] = process[j];
            process[j] = temp;
            i++;
            j--;            
        }
    }

    if(left < j){
        quickSort(deadline,process,left,j);
    }
    if(i < right){
        quickSort(deadline,process,i,right);
    }
}



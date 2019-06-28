#include "FileParser.h"
#include <iostream>
#include <fstream>


FileParser:: FileParser(){
}

FileParser::~FileParser(){
}

bool FileParser::fileOpen(){
    return openFile;
}

Process FileParser::parseFile(string inputFile){
    if(inputFile!=""){
        string fileLocation = "./Process_Files/";
        fileName = fileLocation + inputFile;        
    }

    ifstream processFile(fileName);

    if(processFile.is_open()){
        
        processFile >> totalProc >> procSwitch;
        process.setTotalProcess(totalProc);
        process.setProcessSwitch(procSwitch);

        while(processFile >> procNum >> arrivalTime >> burstTime >>relativeDeadline >> period){

            
            process.setProcessNum(procNum);
            process.setArrivalTime(arrivalTime);
            process.setBurstTime(burstTime);
            process.setRelativeDeadLine(relativeDeadline);
            process.setPeriod(period);
            process.setPriority_RM(period);
            process.setPriority_DM(relativeDeadline);
            
            process.addVectorProcNum(procNum);
            process.addVectorPeriod(period);
            process.addVectorArrivalTime(arrivalTime);
            process.addVectorBurstTime(burstTime);
            process.addVectorRelativeDeadline(relativeDeadline);
            process.addVectorProcess(process);

            process.setRemainingExecutionTime(iterator++,burstTime);
        }
        process.setProcessAvailabilityTime();
        process.setProcessDeadlineTime();
        process.initialiseFinishTime();
        openFile = true;             
    }
    else{
        cout << endl << "!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
                     << "!! File Does Not Exist !!" << endl
                     << "!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl; 
        openFile = false;
    }    

    return process;	
}

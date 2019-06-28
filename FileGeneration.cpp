#include "FileGeneration.h"
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <random>

using namespace std;

FileGeneration::FileGeneration(){
}

FileGeneration::~FileGeneration(){
}

void FileGeneration::generateInputFile(string fileName, bool isDistributed){
    srand(time(NULL));
    totalProc = rand() % (MAX_PROCESSES - 6)+ 5; //rand() % ( high - low + 1 ) + low

    setFileName(fileName); 
    setTotalProc(totalProc); 

    if(fileExists(this->fileName)){
        cout << endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
                     << "!! That File Name Is Taken !!" << endl
                     << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
    }
    else{
        cout << endl << "================================" << endl
                     << "==      FILE GENERATION       ==" << endl
                     << "================================" << endl << endl;
        generateFile(isDistributed);
        cout << endl << "================================" << endl
                     << "==   FINISH FILE GENERATION   ==" << endl
                     << "================================" << endl << endl;
        }      
}

void FileGeneration::generateInputFile(string fileName, int totalProc, bool isDistributed){


    setFileName(fileName); 
    setTotalProc(totalProc); 

    if(totalProc >= 5){
        if(fileExists(this->fileName)){
            cout << endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
                         << "!! That File Name Is Taken !!" << endl
                         << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
        }
        else{
            cout << endl << "================================" << endl
                         << "==      FILE GENERATION       ==" << endl
                         << "================================" << endl << endl;
            generateFile(isDistributed);
            cout << endl << "================================" << endl
                         << "==   FINISH FILE GENERATION   ==" << endl
                         << "================================" << endl << endl;
        }        
    }    
}

void FileGeneration::generateFile(bool isDistributed){
    outputFile.open(fileName); 

    if(isDistributed){
        procSwitch = 5;
    }
    else{
        procSwitch = 0;       
    }

    outputFile << totalProc << " " << procSwitch << endl;
    cout << totalProc << " " << procSwitch << endl;

    for(procNum; procNum <= totalProc; procNum++){

        (isDistributed) ? randomizeDist():randomizeNotDist();        
        
        outputFile << procNum << " " << arrivalTime << " " << burstTime << " " << relativeDealine << " " << period << endl;
        cout << procNum << " " << arrivalTime << " " << burstTime << " " << relativeDealine << " " << period << endl;
    }
    
    outputFile.close();

    

}

void FileGeneration::setFileName(string fileName){
    this->fileName = "./Process_Files/" + fileName;
}

void FileGeneration::setTotalProc(int totalProc){
    this->totalProc = totalProc;
}

bool FileGeneration::fileExists(const string& fileName){
    struct stat buffer;
    return (stat (fileName.c_str(), &buffer) == 0);
}

void FileGeneration::randomizeDist(){
    // @mt19937 - produces high quality unsigned integer random numbers 
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<double> exp_dist(1);
    
    arrivalTime = exp_dist(gen) * 20;
    randomizeNotDist();
}

void FileGeneration::randomizeNotDist(){
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> uni_dist_burstTime(30,100);
    uniform_int_distribution<int> uni_dist_x(1,500);
    uniform_int_distribution<int> uni_dist_y(1,1000);

    burstTime = uni_dist_burstTime(gen);

    int x = uni_dist_x(gen);
    relativeDealine = burstTime + x;

    int y = uni_dist_y(gen);
    period = relativeDealine + y;
}
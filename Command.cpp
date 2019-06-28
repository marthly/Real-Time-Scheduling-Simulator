#include "Command.h"
#include <iostream>
#include <string>

Command::Command(){
}

Command::~Command(){
}

string Command::getFileName(){
    return fileName;
}

string Command::getAlgorithm(){
    return algorithm;
}


void Command::parse(string input){
    int charPos = 0, wordIndex = 0;
    string temp;
    char eof = '\000', blankSpace = ' ';

    while(input[charPos] != eof) {
        while((input[charPos] != blankSpace) && (input[charPos] != eof)){
            temp += input[charPos];                
            charPos++;     

            if(temp == "-a" || temp == "-n"){
                temp+= " ";
                charPos++;
            }

            if(temp == "<" || temp == ">"){
                charPos++;
            }


        }

        args.push_back(temp);
        temp = "";

        while(input[charPos] == blankSpace){
            charPos++;
        }
        wordIndex++;
    }
    
	if(input != "exit" && args[0] == "sim"){
        while(!args.empty()){
            temp = args.back();
            if(temp[0] == *"<"){
                fileName = args.back().replace(0,1,"");
                args.pop_back();
                simRunnable = true;
            }
            else if((temp == "-a RM") || (temp == "-a DM") || (temp == "-a EDF")){
                algorithm = temp;
                algorithmPresent = true;
                args.pop_back();
            }
            else if(temp == "-v"){
                verbose = true;
                args.pop_back();
            }
            else if(temp == "-d"){
                detailed = true;
                args.pop_back();
            }
            else if(temp == "sim"){
                cmd = temp;
                args.pop_back();
                
            }
            else{
                cout << endl << "COMMAND IS INCORRECTLY FORMATED" << endl
                     << "=========================================================" << endl
                     << "Template: $ sim [-d] [-v] [-a algorithm] < [input_file]" << endl
                     << "algorithm: RM, DM, EDF" << endl
                     << "=========================================================" << endl << endl;
                     simRunnable = false;
                     break;
            }
        }        
    }
    else if(args[0] == "gen" && args.back()[0] == *">"){

        //displayInput();
        
        while(!args.empty()){
            temp = args.back();
            if(temp[0] == *">"){
                fileName = args.back().replace(0,1,"");
                args.pop_back();
                genRunnable = true;
            }
            else if(temp == "-d"){
                distributed = true;
                args.pop_back();
            }
            else if(args.back()[0] == *"-" && args.back()[1] == *"n"){
                temp = args.back().replace(0,3,"");
                try{
                    totalProc = stoi(temp);
                    if(totalProc < 5){
                        cout << endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
                             << "!!! Must Enter Atleast 5 Processes !!!" << endl
                             << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
                    }
                }
                catch(const invalid_argument& e){
                    cout << "You Did Not Enter A Number In The [-n number_of_process] Argument" << endl << endl;
                }
                args.pop_back();
            }
            else if(temp == "gen"){
                cmd = temp;
                args.pop_back();                    
            }
            else{
                cout << endl << "COMMAND IS INCORRECTLY FORMATED" << endl
                     << "=========================================================" << endl
                     << "Template: $ gen [-n number_of_process] [-d] > [output_file]" << endl
                     << "number_of_process: must be atleast 5" << endl
                     << "=========================================================" << endl << endl;
                     genRunnable = false;
                     break;
            }
        }
        if(genRunnable){
            if(totalProc == -999){
                fileGeneration.generateInputFile(fileName,distributed);    
            }
            else{
                fileGeneration.generateInputFile(fileName, totalProc, distributed);
            }            
        } 
    }
    else{
        cout << endl << "VALID COMMANDS" << endl
             << "==============================================" << endl
             << " gen [-n number_of_process] > [output_file]" << endl
             << " number_of_process: must be atleast 5" << endl
             << "==============================================" << endl
             << " sim [-d] [-v] [-a algorithm] < [input_file]" << endl
             << " algorithm: RM, DM, EDF" << endl
             << "==============================================" << endl << endl;
    }
}

void Command::displayInput(){
    cout << "Command Entered: ";

    for(unsigned int i = 0; i < args.size(); i++){
        cout << args[i] <<  " ";
    }
    cout << endl;
}

bool Command::isVerbose(){
    return verbose;
}

bool Command::isDetailed(){
    return detailed;
}

bool Command::isAlgorithmPresent(){
    return algorithmPresent;
}

bool Command::isSimRunnable(){
    return simRunnable;
}

bool Command::isGenRunnable(){
    return genRunnable;
}
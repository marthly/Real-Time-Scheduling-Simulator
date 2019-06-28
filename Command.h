#ifndef COMMAND_H
#define COMMAND_H

#include "FileGeneration.h"
#include <iostream>
#include <vector>

using namespace std;

class Command{
public:

	Command();
	~Command();

	string getAlgorithm();
	string getFileName();

	void parse(string input);
	void displayInput();

	bool isVerbose();
	bool isDetailed();
	bool isAlgorithmPresent();
	bool isSimRunnable();
	bool isGenRunnable();

	
private:

	FileGeneration fileGeneration;

	vector<string> args;
	string cmd, fileName, algorithm;
	int totalProc = -999;
	bool verbose = false, detailed = false, distributed = false, algorithmPresent = false; 
	bool simRunnable = false, genRunnable = false;	
};



#endif
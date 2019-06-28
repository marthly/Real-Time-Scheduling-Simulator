#include "Execution.h"
#include <iostream>
#include <iomanip>

Execution::Execution(){
}

Execution::~Execution(){
}        

void Execution::execute(string inputCommand){
	command.parse(inputCommand);
	if(command.isSimRunnable()){
		process = fileParser.parseFile(command.getFileName());
		if(fileParser.fileOpen()){
			if(process.isLCM_LIMIT_PASSED()){
        		cout << endl << "!! LCM Value =  " << process.getLCM_Value() << " !!" << endl
                     		 << "!! LCM will be limited to " << process.getLCM_LIMIT_VALUE() <<  " !!" << endl << endl;
			}

			initialiseResultsFile();

			if(command.getAlgorithm() == "-a RM"){
				rateMonotonic(process);
			}
			else if(command.getAlgorithm() == "-a EDF"){
				earlyDeadline(process);
			}
			else if(command.getAlgorithm() == "-a DM"){
				deadlineMono(process);
			}
			else if(!command.isVerbose()&&!command.isDetailed() && command.getAlgorithm() == ""){
				rateMonotonic(process);
				resultsFile << "==================================================================================" << endl << endl;
					cout << "=================================================================================="  << endl << endl;
				earlyDeadline(process);
				resultsFile << "==================================================================================" << endl << endl;
					cout << "=================================================================================="  << endl << endl;
				deadlineMono(process);
			}
			else{
				cout << endl << "!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
						 	 << "!! SELECT AN ALGORITHM !!" << endl
							 << "!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl; 
			}

			resultsFile << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl
						<< "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;   
				cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl
						<< "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;	
		}
	}	
}

void Execution::rateMonotonic(Process process){

	resultsFile << "========================================" << endl
		 		<< "Rate Monotonic Scheduling Algorithm(RM): " << endl
		 		<< "========================================" << endl;

		   cout << "========================================" << endl
				<< "Rate Monotonic Scheduling Algorithm(RM): " << endl
				<< "========================================" << endl;
	if(process.isSuffCond_RM() && process.isNecCond_RM()){
		rateMonotonicAlg(process);

	}
	else if(!process.isSuffCond_RM() && process.isNecCond_RM()){
		rateMonotonicAlg(process);
	}
	else{
		resultsFile << "There is no feasible schedule to be produced." << endl << endl;
			   cout << "There is no feasible schedule to be produced." << endl << endl;
	}
}

void Execution::rateMonotonicAlg(Process process){	
	while(currentTime <= process.getTotalScheduleTime()){

		if(currentTime <= process.getTotalScheduleTime()){
			//Add Process to CPU Bound vector for execution if they arrive at current time
			for(int procNum = 1; procNum <= process.getTotalProcess();procNum++){
				int availableTime, deadlineTime;

				if(process.getProcessAvailabilitySize(procNum) > 0){	

					availableTime = process.getProcessAvailabilityTime(procNum).front();

					if(process.getProcessDeadlineTime(procNum).size() > 0){
						deadlineTime = process.getProcessDeadlineTime(procNum).front();
					}

					if(availableTime <= currentTime){

						vectorCPUBound.push_back(procNum);
						process.removeAvailableTime(procNum);

						if(process.getProcessDeadlineTime(procNum).size() > 0){
							process.setRemainingDeadline(procNum, deadlineTime);
							process.removeDeadlineTime(procNum);
						}
					}
				}
			}		


			//Finding the next available process with the highest priority
			int availableTime, availableTime2; 
			float availablePriority, availablePriority2;
			
			nextAvailableProcTime = 9999;
			nextAvailableProcPriority = 0;
			//Find the  next process with the smallest available time and highest priority
			for(int procNum = 1; procNum <= process.getTotalProcess();procNum++){
				
				if(process.getProcessAvailabilitySize(procNum) > 0){
					availableTime = nextAvailableProcTime;
					availableTime2 = process.getProcessAvailabilityTime(procNum).front();

					availablePriority = nextAvailableProcPriority;
					availablePriority2 = process.getPriority_RM(procNum);

					if(availableTime <= availableTime2){	//Checks if one available time is less than or equal to the other available
						nextAvailableProcTime = availableTime;
						nextAvailableProcPriority = availablePriority;
						if(availablePriority == availablePriority2){//If equal
							if(availablePriority >= availablePriority2){//Checks which has the larger priority
								nextAvailableProcTime = availableTime;
								nextAvailableProcPriority = availablePriority;
							}
						}
					}
					else if(availableTime2 <= availableTime){
						nextAvailableProcTime = availableTime2;
						nextAvailableProcPriority = availablePriority2;
						if(availablePriority2 == availablePriority){
							if(availablePriority2 >= availablePriority){
								nextAvailableProcTime = availableTime2;
								nextAvailableProcPriority = availablePriority2;
							}
						}
					}		
				}	
			}

			//Find the next process number of the next available process
			for(int i = 1; i <= process.getTotalProcess(); i++){
				if(nextAvailableProcTime == process.getProcessAvailabilityTime(i).front() && nextAvailableProcPriority == process.getPriority_RM(i)){
					nextAvailableProc = i;
					i = process.getTotalProcess();
				}
			}
		}
		
		
		//Select process with the highest(largest) priority that is in the CPUBoundVector ready to be executed
		if(vectorCPUBound.size() >= 1){

			if(vectorCPUBound.size() >= 2){

				int procNum, otherProcNum;	

				currentRunningProc = vectorCPUBound[0];
				//Find the process with the highest(largest) priority
				for(int procPos = 1; procPos < vectorCPUBound.size();procPos++){			
					procNum = currentRunningProc;
					otherProcNum = vectorCPUBound[procPos];

					currentRunningProc = (process.getPriority_RM(procNum) > process.getPriority_RM(otherProcNum)) ? procNum:otherProcNum;
				}

				//Find the position of the process with the highest(largest) priority
				for(int i = 0; i < vectorCPUBound.size(); i++){
					if(currentRunningProc == vectorCPUBound[i]){
						currentRunningProcPosition = i;
					}
				}
			}
			else if(vectorCPUBound.size() == 1){
				currentRunningProc = vectorCPUBound[0];
				currentRunningProcPosition = 0;
			}

			currentRunningProcBurstTime = process.getRemainingExecutionTime(currentRunningProc);
			

			//Display waiting processes
			if(vectorCPUBound.size() > 1 && command.isVerbose()){
				resultsFile << "Waiting Processes At Time "<< currentTime << ": [";
					   cout << "Waiting Processes At Time "<< currentTime << ": [";
				for(int i = 0; i < vectorCPUBound.size();i++){
					if(i < vectorCPUBound.size() - 1){
						resultsFile << vectorCPUBound[i] << ", ";
							   cout << vectorCPUBound[i] << ", ";
					}
					else if(i == vectorCPUBound.size() - 1){
						resultsFile << vectorCPUBound[i] << "]" << endl << endl;
							   cout << vectorCPUBound[i] << "]" << endl << endl;
					}
				}
			}
			else if(vectorCPUBound.size() == 1 && command.isVerbose()){
				resultsFile << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
					   cout << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
			}

			removeFromCPUBound(currentRunningProcPosition);

			//When NO pre-emption occurs
			if(currentTime + currentRunningProcBurstTime <= nextAvailableProcTime){

				startTime = currentTime;
				currentTime += process.getRemainingExecutionTime(currentRunningProc);
				if(command.isVerbose()){
					verboseView(process);
				}

				process.setFinishTime(currentRunningProc, currentTime);
				process.setRemainingExecutionTime(currentRunningProc,process.getBurstTime(currentRunningProc));				
			}
			//When pre-emption occurs
			else{
				startTime = currentTime;
				currentTime = nextAvailableProcTime;			
				partiallyExecutedTime = currentTime - startTime;
				remainingExecutionTime = process.getRemainingExecutionTime(currentRunningProc);

				preEmpted = true;
				if(command.isVerbose()){
					verboseView(process);
				}
				preEmpted = false;

				vectorCPUBound.push_back(currentRunningProc);
				process.setRemainingExecutionTime(currentRunningProc, remainingExecutionTime - partiallyExecutedTime);


				if(process.getProcessSwitch() > 0){
					if(command.isVerbose()){
						resultsFile << process.getProcessSwitch() << " Time Unit Delay Due To Pre-Emption." << endl << endl;
							   cout << process.getProcessSwitch() << " Time Unit Delay Due To Pre-Emption." << endl << endl;
					}					
					currentTime = nextAvailableProcTime + process.getProcessSwitch();
				}
			}
		}

		
		//Check if the process missed it deadline
		if(currentTime > process.getRemainingDeadline(currentRunningProc)){
			if(command.isVerbose()){
				resultsFile << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
						cout << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
			}
			
			//Check for duplicate
			for(int i = 0; i < process.getVectorMissedProcess().size();i++){
				if(process.getVectorMissedProcess()[i] == currentRunningProc){
					missingProcDuplicate = true;
				}
			}

			//Add missed process to missed process to storage if no duplicate found
			if(!missingProcDuplicate){
				process.addVectorMissedProcess(currentRunningProc);
				process.addVectorMissedProcessFinishTime(currentTime);
				process.addVectorMissedProcessExpectedFinishTime(process.getRemainingDeadline(currentRunningProc));
			}				
			ganttPass = false;
		}	
		missingProcDuplicate = false;	
		

		requiredCPUTime = currentTime;	

		//When the CPU is idle
		checkIdle();

		if(command.isVerbose()){
			resultsFile << "=================================================================================="  << endl << endl;
				   cout << "=================================================================================="  << endl << endl;
		}	

		while(currentTime > process.getTotalScheduleTime() && vectorCPUBound.size() > 0){
			//Select process with the highest(largest) priority that is in the queue ready to be executed
			if(vectorCPUBound.size() >= 1){

				if(vectorCPUBound.size() >= 2){

					int procNum, otherProcNum;	

					currentRunningProc = vectorCPUBound[0];
					//Find the process with the highest(largest) priority
					for(int procPos = 1; procPos < vectorCPUBound.size();procPos++){			
						procNum = currentRunningProc;
						otherProcNum = vectorCPUBound[procPos];

						currentRunningProc = (process.getPriority_RM(procNum) > process.getPriority_RM(otherProcNum)) ? procNum:otherProcNum;
					}

					//Find the position of the process with the highest(largest) priority
					for(int i = 0; i < vectorCPUBound.size(); i++){
						if(currentRunningProc == vectorCPUBound[i]){
							currentRunningProcPosition = i;
						}
					}
				}
				else if(vectorCPUBound.size() == 1){
					currentRunningProc = vectorCPUBound[0];
					currentRunningProcPosition = 0;
				}

				currentRunningProcBurstTime = process.getRemainingExecutionTime(currentRunningProc);
				

				//Display waiting processes
				if(vectorCPUBound.size() > 1 && command.isVerbose()){
					resultsFile << "Waiting Processes At Time "<< currentTime << ": [";
						cout << "Waiting Processes At Time "<< currentTime << ": [";
					for(int i = 0; i < vectorCPUBound.size();i++){
						if(i < vectorCPUBound.size() - 1){
							resultsFile << vectorCPUBound[i] << ", ";
								cout << vectorCPUBound[i] << ", ";
						}
						else if(i == vectorCPUBound.size() - 1){
							resultsFile << vectorCPUBound[i] << "]" << endl << endl;
								cout << vectorCPUBound[i] << "]" << endl << endl;
						}
					}
				}
				else if(vectorCPUBound.size() == 1 && command.isVerbose()){
					resultsFile << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
						cout << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
				}

				removeFromCPUBound(currentRunningProcPosition);

				startTime = currentTime;
				currentTime += process.getRemainingExecutionTime(currentRunningProc);
				if(command.isVerbose()){
					verboseView(process);
				}

				process.setFinishTime(currentRunningProc, currentTime);
				process.setRemainingExecutionTime(currentRunningProc,process.getBurstTime(currentRunningProc));					
			}

			
			//Check if the process missed it deadline
			if(currentTime > process.getRemainingDeadline(currentRunningProc)){
				if(command.isVerbose()){
					resultsFile << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
							cout << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
				}

				//Checking for duplicates
				for(int i = 0; i < process.getVectorMissedProcess().size();i++){
					if(process.getVectorMissedProcess()[i] == currentRunningProc){
						missingProcDuplicate = true;
					}
				}

				//Add missed process to missed process to storage if no duplicate found
				if(!missingProcDuplicate){
					process.addVectorMissedProcess(currentRunningProc);
					process.addVectorMissedProcessFinishTime(currentTime);
					process.addVectorMissedProcessExpectedFinishTime(process.getRemainingDeadline(currentRunningProc));
				}				
				ganttPass = false;
			}	
			missingProcDuplicate = false;	
			

			requiredCPUTime = currentTime;	

			//When the CPU is idle
			checkIdle();

			if(command.isVerbose()){
				resultsFile << "=================================================================================="  << endl << endl;
					cout << "=================================================================================="  << endl << endl;
			}	
		}				
	}

	if(command.isDetailed()){
		detailedView(process);
	}
	else if(!command.isDetailed() && !command.isVerbose()){
		minimalView(process);
	}

	report(process);
	resetVariable();
}

void Execution::earlyDeadline(Process process){

	resultsFile << "=======================================" << endl
		 		<< "Earliest Deadline First Algorithm(EDF):" << endl
		 		<< "=======================================" << endl;

		   cout << "=======================================" << endl
				<< "Earliest Deadline First Algorithm(EDF):" << endl
				<< "=======================================" << endl;
	if(process.isSuffCond_EDF() && process.isNecCond_EDF()){
		earlyDeadlineAlg(process);
	}
	else if(!process.isSuffCond_EDF() && process.isNecCond_EDF()){
		earlyDeadlineAlg(process);
	}
	else{
		resultsFile << "There is no feasible schedule to be produced." << endl << endl;
			   cout << "There is no feasible schedule to be produced." << endl << endl;
	}	
}

void Execution::earlyDeadlineAlg(Process process){
	while(currentTime <= process.getTotalScheduleTime()){			

		if(currentTime <= process.getTotalScheduleTime()){
			//Add Process to CPU Bound vector for execution if they arrive at current time
			for(int procNum = 1; procNum <= process.getTotalProcess();procNum++){		

				if(process.getProcessAvailabilitySize(procNum) > 0){
					int availableTime, deadlineTime;

					availableTime = process.getProcessAvailabilityTime(procNum).front();

					if(process.getProcessDeadlineTime(procNum).size() > 0){
						deadlineTime = process.getProcessDeadlineTime(procNum).front();
					}			 		

					if(availableTime <= currentTime){

						vectorCPUBound.push_back(procNum);
						process.removeAvailableTime(procNum);
		
						if(process.getProcessDeadlineTime(procNum).size() > 0){		
							process.setRemainingDeadline(procNum, deadlineTime);		
							process.removeDeadlineTime(procNum);
						}
					}
				}			
			}			

			int availableTime, availableTime2, availableDeadline, availableDeadline2;
			
			nextAvailableProcTime = 9999;
			nextAvailableProcDeadline = 9999;
			//Find the next process with the smallest available time and smallest deadline time
			for(int procNum = 1; procNum <= process.getTotalProcess();procNum++){
				
				if(process.getProcessAvailabilitySize(procNum) > 0){
					availableTime = nextAvailableProcTime;
					availableTime2 = process.getProcessAvailabilityTime(procNum).front();

					if(process.getProcessDeadlineTime(procNum).size() > 0){
						availableDeadline = nextAvailableProcDeadline;
						availableDeadline2 = process.getProcessDeadlineTime(procNum).front();
					}				

					if(availableTime <= availableTime2){		
						nextAvailableProcTime = availableTime;
						if(process.getProcessDeadlineTime(procNum).size() > 0){
							nextAvailableProcDeadline = availableDeadline;
						}
					}
					else if(availableTime2 <= availableTime){
						nextAvailableProcTime = availableTime2;
						if(process.getProcessDeadlineTime(procNum).size() > 0){
							nextAvailableProcDeadline = availableDeadline2;
						}		
					}
				}					
			}

			//Find the next process number of the next available process
			for(int i = 1; i <= process.getTotalProcess(); i++){
				if(nextAvailableProcTime == process.getProcessAvailabilityTime(i).front() && nextAvailableProcDeadline == process.getProcessDeadlineTime(i).front()){
					nextAvailableProc = i;
					i = process.getTotalProcess();
				}
			}
		}	
		
		
		//Select process with the smallest deadline time that is in the queue, ready to be executed
		if(vectorCPUBound.size() >= 1){

			if(vectorCPUBound.size() >= 2){

				int procNum, otherProcNum;	
				
				currentRunningProc = vectorCPUBound[0];
				//Find the process with the smallest deadline time
				for(int procPos = 1; procPos < vectorCPUBound.size();procPos++){	
					procNum = currentRunningProc;
					otherProcNum = vectorCPUBound[procPos];

					currentRunningProc = (process.getRemainingDeadline(procNum) < process.getRemainingDeadline(otherProcNum)) ? procNum:otherProcNum;
				}
				

				//Find the position of the process with the smallest deadline time
				for(int i = 0; i < vectorCPUBound.size(); i++){
					if(currentRunningProc == vectorCPUBound[i]){
						currentRunningProcPosition = i;
						i = vectorCPUBound.size();
					}
				}
			}
			else if(vectorCPUBound.size() == 1){
				currentRunningProc = vectorCPUBound[0];
				currentRunningProcPosition = 0;
			}
			
			//Display waiting processes
			if(vectorCPUBound.size() > 1 && command.isVerbose()){
				resultsFile << "Waiting Processes At Time "<< currentTime << ": [";
					cout << "Waiting Processes At Time "<< currentTime << ": [";
				for(int i = 0; i < vectorCPUBound.size();i++){
					if(i < vectorCPUBound.size() - 1){
						resultsFile << vectorCPUBound[i] << ", ";
							cout << vectorCPUBound[i] << ", ";
					}
					else if(i == vectorCPUBound.size() - 1){
						resultsFile << vectorCPUBound[i] << "]" << endl << endl;
							cout << vectorCPUBound[i] << "]" << endl << endl;
					}
				}
			}
			else if(vectorCPUBound.size() == 1 && command.isVerbose()){
				resultsFile << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
					cout << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
				
			}

			currentRunningProcBurstTime = process.getRemainingExecutionTime(currentRunningProc);	
					
			removeFromCPUBound(currentRunningProcPosition);
		
			//When NO pre-emption occurs
			if(currentTime + currentRunningProcBurstTime <= nextAvailableProcTime){
				startTime = currentTime;
				currentTime += process.getRemainingExecutionTime(currentRunningProc);
				
				if(command.isVerbose()){
					verboseView(process);
				}
				process.setFinishTime(currentRunningProc, currentTime);			
				process.setRemainingExecutionTime(currentRunningProc,process.getBurstTime(currentRunningProc));
			}
			//When pre-emption occurs
			else{
				startTime = currentTime;
				currentTime = nextAvailableProcTime;			
				partiallyExecutedTime = currentTime - startTime;
				remainingExecutionTime = process.getRemainingExecutionTime(currentRunningProc);

				preEmpted = true;
				if(command.isVerbose()){
					verboseView(process);
				}
				preEmpted = false;

				vectorCPUBound.push_back(currentRunningProc);
				process.setRemainingExecutionTime(currentRunningProc, remainingExecutionTime - partiallyExecutedTime);


				if(process.getProcessSwitch() > 0){
					if(command.isVerbose()){
						resultsFile << process.getProcessSwitch() << " Time Unit Delay Due To Pre-Emption." << endl << endl;
							cout << process.getProcessSwitch() << " Time Unit Delay Due To Pre-Emption." << endl << endl;
					}	
					currentTime = nextAvailableProcTime + process.getProcessSwitch();
				}
			}
		}

		
		//Check if the process missed it deadline
		if(currentTime > process.getRemainingDeadline(currentRunningProc)){
			if(command.isVerbose()){
				resultsFile << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
						cout << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
			}

			//Checking for duplicates
			for(int i = 0; i < process.getVectorMissedProcess().size();i++){
				if(process.getVectorMissedProcess()[i] == currentRunningProc){
					missingProcDuplicate = true;
				}
			}

			//Add missed process to missed process to storage if no duplicate found
			if(!missingProcDuplicate){
				process.addVectorMissedProcess(currentRunningProc);
				process.addVectorMissedProcessFinishTime(currentTime);
				process.addVectorMissedProcessExpectedFinishTime(process.getRemainingDeadline(currentRunningProc));
			}				
			ganttPass = false;
		}	
		missingProcDuplicate = false;
					

		requiredCPUTime = currentTime;

		//When the CPU is idle
		checkIdle();

		if(command.isVerbose()){
			resultsFile << "=================================================================================="  << endl << endl;		
				cout << "=================================================================================="  << endl << endl;		
		}

		while(currentTime > process.getTotalScheduleTime() && vectorCPUBound.size() > 0){
			if(vectorCPUBound.size() >= 1){

				if(vectorCPUBound.size() >= 2){

					int procNum, otherProcNum;	
					
					currentRunningProc = vectorCPUBound[0];
					//Find the process with the smallest deadline time
					for(int procPos = 1; procPos < vectorCPUBound.size();procPos++){	
						procNum = currentRunningProc;
						otherProcNum = vectorCPUBound[procPos];

						currentRunningProc = (process.getRemainingDeadline(procNum) < process.getRemainingDeadline(otherProcNum)) ? procNum:otherProcNum;
					}
					

					//Find the position of the process with the smallest deadline time
					for(int i = 0; i < vectorCPUBound.size(); i++){
						if(currentRunningProc == vectorCPUBound[i]){
							currentRunningProcPosition = i;
							i = vectorCPUBound.size();
						}
					}
				}
				else if(vectorCPUBound.size() == 1){
					currentRunningProc = vectorCPUBound[0];
					currentRunningProcPosition = 0;
				}
				
				//Display waiting processes
				if(vectorCPUBound.size() > 1 && command.isVerbose()){
					resultsFile << "Waiting Processes At Time "<< currentTime << ": [";
						cout << "Waiting Processes At Time "<< currentTime << ": [";
					for(int i = 0; i < vectorCPUBound.size();i++){
						if(i < vectorCPUBound.size() - 1){
							resultsFile << vectorCPUBound[i] << ", ";
								cout << vectorCPUBound[i] << ", ";
						}
						else if(i == vectorCPUBound.size() - 1){
							resultsFile << vectorCPUBound[i] << "]" << endl << endl;
								cout << vectorCPUBound[i] << "]" << endl << endl;
						}
					}
				}
				else if(vectorCPUBound.size() == 1 && command.isVerbose()){
					resultsFile << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
						cout << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
					
				}

				currentRunningProcBurstTime = process.getRemainingExecutionTime(currentRunningProc);	
						
				removeFromCPUBound(currentRunningProcPosition);
						
				startTime = currentTime;
				currentTime += process.getRemainingExecutionTime(currentRunningProc);
				
				if(command.isVerbose()){
					verboseView(process);
				}
				process.setFinishTime(currentRunningProc, currentTime);			
				process.setRemainingExecutionTime(currentRunningProc,process.getBurstTime(currentRunningProc));			
			}

			//Check if the process missed it deadline
			if(currentTime > process.getRemainingDeadline(currentRunningProc)){
				if(command.isVerbose()){
					resultsFile << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
							cout << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
				}

				//Checking for duplicates
				for(int i = 0; i < process.getVectorMissedProcess().size();i++){
					if(process.getVectorMissedProcess()[i] == currentRunningProc){
						missingProcDuplicate = true;
					}
				}

				//Add missed process to missed process to storage if no duplicate found
				if(!missingProcDuplicate){
					process.addVectorMissedProcess(currentRunningProc);
					process.addVectorMissedProcessFinishTime(currentTime);
					process.addVectorMissedProcessExpectedFinishTime(process.getRemainingDeadline(currentRunningProc));
				}				
				ganttPass = false;
			}	
			missingProcDuplicate = false;
					

			requiredCPUTime = currentTime;

			//When the CPU is idle
			checkIdle();

			if(command.isVerbose()){
				resultsFile << "=================================================================================="  << endl << endl;		
					cout << "=================================================================================="  << endl << endl;		
			}
		}		
	}

	if(command.isDetailed()){
		detailedView(process);
	}
	else if(!command.isDetailed() && !command.isVerbose()){
		minimalView(process);
	}

	report(process);
	resetVariable();
}

void Execution::deadlineMono(Process process){

	resultsFile << "============================================" << endl
				<< "Deadline Monotonic Scheduling Algorithm(DM): " << endl
				<< "============================================" << endl;

		   cout << "============================================" << endl
				<< "Deadline Monotonic Scheduling Algorithm(DM): " << endl
				<< "============================================" << endl;	

	if(process.isSuffCond_DM() && process.isNecCond_DM()){		
		deadlineMonoAlg(process);
	}
	else if(process.isSuffCond_DM() && !process.isNecCond_DM()){
		deadlineMonoAlg(process);
	}
	else{
		resultsFile << "There is no feasible schedule to be produced." << endl << endl;
			   cout << "There is no feasible schedule to be produced." << endl << endl;

	}	
}

void Execution::deadlineMonoAlg(Process process){	
	while(currentTime <= process.getTotalScheduleTime()){		

		if(currentTime <= process.getTotalScheduleTime()){
			//Add Process to CPU Bound vector for execution if they arrive at current time
			for(int procNum = 1; procNum <= process.getTotalProcess();procNum++){
				int availableTime, deadlineTime;

				if(process.getProcessAvailabilitySize(procNum) > 0){	

					availableTime = process.getProcessAvailabilityTime(procNum).front();

					if(process.getProcessDeadlineTime(procNum).size() > 0){
						deadlineTime = process.getProcessDeadlineTime(procNum).front();
					}

					if(availableTime <= currentTime){
						vectorCPUBound.push_back(procNum);
						process.removeAvailableTime(procNum);

						if(process.getProcessDeadlineTime(procNum).size() > 0){
							process.setRemainingDeadline(procNum, deadlineTime);
							process.removeDeadlineTime(procNum);
						}
					}
				}
			}		


			//Finding the next available process with the highest priority
			int availableTime, availableTime2;
			float availablePriority, availablePriority2;
			
			nextAvailableProcTime = 9999;
			nextAvailableProcPriority = 0;
			//Find the  next process with the smallest available time and highest priority
			for(int procNum = 1; procNum <= process.getTotalProcess();procNum++){
				
				if(process.getProcessAvailabilitySize(procNum) > 0){
					availableTime = nextAvailableProcTime;
					availableTime2 = process.getProcessAvailabilityTime(procNum).front();

					availablePriority = nextAvailableProcPriority;
					availablePriority2 = process.getPriority_DM(procNum);

					if(availableTime <= availableTime2){	//Checks if one available time is less than or equal to the other available
						nextAvailableProcTime = availableTime;
						nextAvailableProcPriority = availablePriority;
						if(availablePriority == availablePriority2){//If equal
							if(availablePriority >= availablePriority2){//Checks which has the smaller deadline
								nextAvailableProcTime = availableTime;
								nextAvailableProcPriority = availablePriority;
							}
						}
					}
					else if(availableTime2 <= availableTime){
						nextAvailableProcTime = availableTime2;
						nextAvailableProcPriority = availablePriority2;
						if(availablePriority2 == availablePriority){
							if(availablePriority2 >= availablePriority){
								nextAvailableProcTime = availableTime2;
								nextAvailableProcPriority = availablePriority2;
							}
						}
					}		
				}		
			}

			//Find the next process number of the next available process
			for(int i = 1; i <= process.getTotalProcess(); i++){
				if(nextAvailableProcTime == process.getProcessAvailabilityTime(i).front() && nextAvailableProcPriority == process.getPriority_DM(i)){
					nextAvailableProc = i;
					i = process.getTotalProcess();
				}
			}		
		}
		
		
		//Select process with the highest(largest) priority that is in the queue ready to be executed
		if(vectorCPUBound.size() >= 1){

			if(vectorCPUBound.size() >= 2){

				int procNum, otherProcNum;	

				currentRunningProc = vectorCPUBound[0];
				//Find the process with the highest(largest) priority
				for(int procPos = 1; procPos < vectorCPUBound.size();procPos++){			
					procNum = currentRunningProc;
					otherProcNum = vectorCPUBound[procPos];

					currentRunningProc = (process.getPriority_DM(procNum) > process.getPriority_DM(otherProcNum)) ? procNum:otherProcNum;
				}

				//Find the position of the process with the highest(largest) priority
				for(int i = 0; i < vectorCPUBound.size(); i++){
					if(currentRunningProc == vectorCPUBound[i]){
						currentRunningProcPosition = i;
					}
				}
			}
			else if(vectorCPUBound.size() == 1){
				currentRunningProc = vectorCPUBound[0];
				currentRunningProcPosition = 0;
			}

			currentRunningProcBurstTime = process.getRemainingExecutionTime(currentRunningProc);
			

			//Display waiting processes
			if(vectorCPUBound.size() > 1 && command.isVerbose()){
				resultsFile << "Waiting Processes At Time "<< currentTime << ": [";
					   cout << "Waiting Processes At Time "<< currentTime << ": [";
				for(int i = 0; i < vectorCPUBound.size();i++){
					if(i < vectorCPUBound.size() - 1){
						resultsFile << vectorCPUBound[i] << ", ";
							   cout << vectorCPUBound[i] << ", ";
					}
					else if(i == vectorCPUBound.size() - 1){
						resultsFile << vectorCPUBound[i] << "]" << endl << endl;
							   cout << vectorCPUBound[i] << "]" << endl << endl;
					}
				}
			}
			else if(vectorCPUBound.size() == 1 && command.isVerbose()){
				resultsFile << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
					   cout << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
			}

			removeFromCPUBound(currentRunningProcPosition);

			//When NO pre-emption occurs
			if(currentTime + currentRunningProcBurstTime <= nextAvailableProcTime){

				startTime = currentTime;
				currentTime += process.getRemainingExecutionTime(currentRunningProc);
				if(command.isVerbose()){
					verboseView(process);
				}

				process.setFinishTime(currentRunningProc, currentTime);
				process.setRemainingExecutionTime(currentRunningProc,process.getBurstTime(currentRunningProc));
			}
			//When pre-emption occurs
			else{
				startTime = currentTime;
				currentTime = nextAvailableProcTime;			
				partiallyExecutedTime = currentTime - startTime;
				remainingExecutionTime = process.getRemainingExecutionTime(currentRunningProc);

				preEmpted = true;
				if(command.isVerbose()){
					verboseView(process);
				}
				preEmpted = false;

				vectorCPUBound.push_back(currentRunningProc);
				process.setRemainingExecutionTime(currentRunningProc, remainingExecutionTime - partiallyExecutedTime);


				if(process.getProcessSwitch() > 0){
					if(command.isVerbose()){
						resultsFile << process.getProcessSwitch() << " Time Unit Delay Due To Pre-Emption." << endl << endl;
							   cout << process.getProcessSwitch() << " Time Unit Delay Due To Pre-Emption." << endl << endl;
					}	
					currentTime = nextAvailableProcTime + process.getProcessSwitch();
				}
			}
		}

		//Check if the process missed it deadline
		if(currentTime > process.getRemainingDeadline(currentRunningProc)){
			if(command.isVerbose()){
				resultsFile << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
						cout << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
			}

			//Checking for duplicates
			for(int i = 0; i < process.getVectorMissedProcess().size();i++){
				if(process.getVectorMissedProcess()[i] == currentRunningProc){
					missingProcDuplicate = true;
				}
			}

			//Add missed process to missed process to storage if no duplicate found
			if(!missingProcDuplicate){
				process.addVectorMissedProcess(currentRunningProc);
				process.addVectorMissedProcessFinishTime(currentTime);
				process.addVectorMissedProcessExpectedFinishTime(process.getRemainingDeadline(currentRunningProc));
			}				
			ganttPass = false;
		}	
		missingProcDuplicate = false;
		

		requiredCPUTime = currentTime;	

		//When the CPU is idle
		checkIdle();

		if(command.isVerbose()){
			resultsFile << "=================================================================================="  << endl << endl;
				   cout << "=================================================================================="  << endl << endl;
		}	

		while(currentTime > process.getTotalScheduleTime() && vectorCPUBound.size() > 0){
			//Select process with the highest(largest) priority that is in the queue ready to be executed
			if(vectorCPUBound.size() >= 1){

				if(vectorCPUBound.size() >= 2){

					int procNum, otherProcNum;	

					currentRunningProc = vectorCPUBound[0];
					//Find the process with the highest(largest) priority
					for(int procPos = 1; procPos < vectorCPUBound.size();procPos++){			
						procNum = currentRunningProc;
						otherProcNum = vectorCPUBound[procPos];

						currentRunningProc = (process.getPriority_DM(procNum) > process.getPriority_DM(otherProcNum)) ? procNum:otherProcNum;
					}

					//Find the position of the process with the highest(largest) priority
					for(int i = 0; i < vectorCPUBound.size(); i++){
						if(currentRunningProc == vectorCPUBound[i]){
							currentRunningProcPosition = i;
						}
					}
				}
				else if(vectorCPUBound.size() == 1){
					currentRunningProc = vectorCPUBound[0];
					currentRunningProcPosition = 0;
				}

				currentRunningProcBurstTime = process.getRemainingExecutionTime(currentRunningProc);
				

				//Display waiting processes
				if(vectorCPUBound.size() > 1 && command.isVerbose()){
					resultsFile << "Waiting Processes At Time "<< currentTime << ": [";
						cout << "Waiting Processes At Time "<< currentTime << ": [";
					for(int i = 0; i < vectorCPUBound.size();i++){
						if(i < vectorCPUBound.size() - 1){
							resultsFile << vectorCPUBound[i] << ", ";
								cout << vectorCPUBound[i] << ", ";
						}
						else if(i == vectorCPUBound.size() - 1){
							resultsFile << vectorCPUBound[i] << "]" << endl << endl;
								cout << vectorCPUBound[i] << "]" << endl << endl;
						}
					}
				}
				else if(vectorCPUBound.size() == 1 && command.isVerbose()){
					resultsFile << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
						cout << "Waiting Processes At Time "<< currentTime << ": [" << vectorCPUBound[0] << "]" << endl << endl;
				}

				removeFromCPUBound(currentRunningProcPosition);				

				startTime = currentTime;
				currentTime += process.getRemainingExecutionTime(currentRunningProc);
				if(command.isVerbose()){
					verboseView(process);
				}

				process.setFinishTime(currentRunningProc, currentTime);
				process.setRemainingExecutionTime(currentRunningProc,process.getBurstTime(currentRunningProc));
				
			}

			//Check if the process missed it deadline
			if(currentTime > process.getRemainingDeadline(currentRunningProc)){
				if(command.isVerbose()){
					resultsFile << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
							cout << "--- PROCESS " << currentRunningProc << " DEADLINE WAS AT TIME " << process.getRemainingDeadline(currentRunningProc) << " ---" << endl << endl;
				}

				//Checking for duplicates
				for(int i = 0; i < process.getVectorMissedProcess().size();i++){
					if(process.getVectorMissedProcess()[i] == currentRunningProc){
						missingProcDuplicate = true;
					}
				}

				//Add missed process to missed process to storage if no duplicate found
				if(!missingProcDuplicate){
					process.addVectorMissedProcess(currentRunningProc);
					process.addVectorMissedProcessFinishTime(currentTime);
					process.addVectorMissedProcessExpectedFinishTime(process.getRemainingDeadline(currentRunningProc));
				}				
				ganttPass = false;
			}	
			missingProcDuplicate = false;	
			

			requiredCPUTime = currentTime;	

			//When the CPU is idle
			checkIdle();

			if(command.isVerbose()){
				resultsFile << "=================================================================================="  << endl << endl;
					cout << "=================================================================================="  << endl << endl;
			}
		}		
	}

	if(command.isDetailed()){
		detailedView(process);
	}
	else if(!command.isDetailed() && !command.isVerbose()){
		minimalView(process);
	}

	report(process);
	resetVariable();
}

void Execution::minimalView(Process process){
	if(ganttPass){
		resultsFile << endl << "Feasibility Report      : ++ FEASIBLE ++" << endl;
			   cout << endl << "Feasibility Report      : ++ FEASIBLE ++" << endl;
	}
	else{
		resultsFile << endl << "Feasibility Report      : -- NOT FEASIBLE --" << endl;
			   cout << endl << "Feasibility Report      : -- NOT FEASIBLE --" << endl;
	}	
		resultsFile << "Total Scheduled Time (T): " << process.getTotalScheduleTime() << endl;
			   cout << "Total Scheduled Time (T): " << process.getTotalScheduleTime() << endl;

	if(ganttPass){
		resultsFile << "Required CPU Time       : " << requiredCPUTime << endl;
		cout << "Required CPU Time       : " << requiredCPUTime << endl;
	}
	else{
		resultsFile << "Required CPU Time       : " << "-" << endl;
		cout << "Required CPU Time       : " << "-" << endl;
	}
		resultsFile << "CPU Utilization         : " << process.getSystemUtilization() * 100 << "%" << endl << endl;	 
		cout << "CPU Utilization         : " << process.getSystemUtilization() * 100 << "%" << endl << endl;

}

void Execution::verboseView(Process process){
	if(!preEmpted){		
		resultsFile << "At time " << startTime << " : Process " << currentRunningProc << " Started Executing and Finished At Time " << currentTime << endl << endl;
			   cout << "At time " << startTime << " : Process " << currentRunningProc << " Started Executing and Finished At Time " << currentTime << endl << endl;
	}
	else{
		if(partiallyExecutedTime > 0){
			resultsFile << "At time " << startTime << " : Process " << currentRunningProc << " Partially Ran for " << partiallyExecutedTime << " Time Unit" << endl;
				   cout << "At time " << startTime << " : Process " << currentRunningProc << " Partially Ran for " << partiallyExecutedTime << " Time Unit" << endl;
		}
		else{
			resultsFile << "At time " << startTime << " : Process " << currentRunningProc << " Tried Running But Was Immediately Stopped." << endl;
				   cout << "At time " << startTime << " : Process " << currentRunningProc << " Tried Running But Was Immediately Stopped." << endl;
		}		
		resultsFile << "At time " << currentTime << " : Process " << currentRunningProc << " Was Pre-Empted By Process " << nextAvailableProc << endl << endl;
			   cout << "At time " << currentTime << " : Process " << currentRunningProc << " Was Pre-Empted By Process " << nextAvailableProc << endl << endl;
	}
}

void Execution::detailedView(Process process){

	resultsFile << endl << "Feasibility Report  	: ";
		   cout << endl << "Feasibility Report  	: ";
	if(ganttPass){
		resultsFile << "++ FEASIBLE ++" << endl;
			   cout << "++ FEASIBLE ++" << endl;
	}
	else{	
		resultsFile << "-- NOT FEASIBLE --" << endl;
			   cout << "-- NOT FEASIBLE --" << endl;
	}

	resultsFile << "Total Scheduled Time (T): " << process.getTotalScheduleTime() << endl;
			cout << "Total Scheduled Time (T): " << process.getTotalScheduleTime() << endl;

	if(ganttPass){
		resultsFile << "Required CPU Time       : " << requiredCPUTime << endl;
			   cout << "Required CPU Time       : " << requiredCPUTime << endl;
	}
	else{
		resultsFile << "Required CPU Time       : " << "-" << endl;
			   cout << "Required CPU Time       : " << "-" << endl;
	}	
		resultsFile << "CPU Utilization         : " << process.getSystemUtilization() * 100 << "%" << endl << endl << endl;
			   cout << "CPU Utilization         : " << process.getSystemUtilization() * 100 << "%" << endl << endl << endl;
		 

	for(int procNum = 1; procNum <= process.getTotalProcess(); procNum++){
		int finishTime;

		resultsFile << "Process " << procNum << endl << "==============================="<< endl
			 		<< "Arrival Time     : " << process.getArrivalTime(procNum) << endl
			 		<< "Burst Time       : " << process.getBurstTime(procNum) << endl
			 		<< "Relative Deadline: " << process.getRelativeDeadline(procNum) << endl
			 		<< "Period           : " << process.getPeriod(procNum) << endl
			 		<< "Finish Time      : [";

			   cout << "Process " << procNum << endl << "==============================="<< endl
					<< "Arrival Time     : " << process.getArrivalTime(procNum) << endl
					<< "Burst Time       : " << process.getBurstTime(procNum) << endl
					<< "Relative Deadline: " << process.getRelativeDeadline(procNum) << endl
					<< "Period           : " << process.getPeriod(procNum) << endl
					<< "Finish Time      : [";

		while(process.getFinishTimeSize(procNum) > 0){
			finishTime = process.getFinishTime(procNum); //getFinishTime pops value afterwards 
			resultsFile << finishTime;
				   cout << finishTime;

			if(process.getFinishTimeSize(procNum)){
				resultsFile << ", ";
					   cout << ", ";
			}
		}
		resultsFile << "]" << endl << endl;
			   cout << "]" << endl << endl;	 
	}
}

void Execution::initialiseResultsFile(){
	string temp = command.getFileName(), fileName, temp1;
	temp1 = temp.replace(temp.end() - 4, temp.end(), "");
	if(temp == ".txt"){
		fileName = "./Process_Files/" + temp1 + "_Output.txt";
	}
	else{
		fileName = "./Process_Files/" + temp + "_Output.txt";
	}
	
	resultsFile.open(fileName, ios_base::app);
}

void Execution::checkIdle(){
	if(vectorCPUBound.size() == 0 && currentTime < nextAvailableProcTime && currentTime != nextAvailableProcTime && nextAvailableProcTime != 9999){

		if(command.isVerbose()){
			resultsFile << "At time " << currentTime << " : ! THE CPU WAS IDLE UNTIL TIME " << nextAvailableProcTime << " !" << endl << endl;
					cout << "At time " << currentTime << " : ! THE CPU WAS IDLE UNTIL TIME " << nextAvailableProcTime << " !" << endl << endl;
		}			
		currentTime = nextAvailableProcTime;
	}
}

void Execution::removeFromCPUBound(int procPos){
	vectorCPUBound.erase(vectorCPUBound.begin() + procPos);
}

void Execution::resetVariable(){
	vectorCPUBound.clear();
	currentTime = 0;
	process.setProcessAvailabilityTime();
	ganttPass = true;
}

void Execution::report(Process process){

	if(command.isVerbose()){
		if(ganttPass){
			resultsFile << "Feasibility Report      : ++ FEASIBLE ++" << endl;
				   cout << "Feasibility Report      : ++ FEASIBLE ++" << endl;		}
		else{
			resultsFile << "Feasibility Report      : -- NOT FEASIBLE --" << endl;
				   cout << "Feasibility Report      : -- NOT FEASIBLE --" << endl;
		}
			resultsFile << "Total Scheduled Time (T): " << process.getTotalScheduleTime() << endl;
				   cout << "Total Scheduled Time (T): " << process.getTotalScheduleTime() << endl;

		if(ganttPass){
			resultsFile << "Required CPU Time       : " << requiredCPUTime << endl;
				   cout << "Required CPU Time       : " << requiredCPUTime << endl;
		}
		else{
			resultsFile << "Required CPU Time       : " << "-" << endl;
				   cout << "Required CPU Time       : " << "-" << endl;
		}
			resultsFile << "CPU Utilization         : " << process.getSystemUtilization() * 100 << "%" << endl << endl;
				   cout << "CPU Utilization         : " << process.getSystemUtilization() * 100 << "%" << endl << endl;		
	}

	if(process.getVectorMissedProcess().size() > 0){
		int total = 0;

		resultsFile << endl << "PROCESSES THAT MISSED THEIR DEADLINE" << endl
					<< "________________________________________________________" << endl << endl;

				cout << endl << "PROCESSES THAT MISSED THEIR DEADLINE" << endl
					<< "________________________________________________________" << endl << endl;

		
		for(int i = 0; i < process.getVectorMissedProcess().size(); i++){			

			total++;
			resultsFile << "Process " << process.getVectorMissedProcess()[i] << " Missed Deadline Time: "
					<< process.getVectorMissedProcessExpectedFinishTime()[i] << endl;

				cout << "Process " << process.getVectorMissedProcess()[i] << " Missed Deadline Time: "
					<< process.getVectorMissedProcessExpectedFinishTime()[i] << endl;
					
		}

		resultsFile << endl << "Total Missed: " << total << endl;
		cout << endl << "Total Missed: " << total << endl;

		resultsFile << "________________________________________________________" << endl << endl;
				cout << "________________________________________________________" << endl << endl;
	}	
}

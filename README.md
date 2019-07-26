# Real Time Scheduling Simulator<br />

## Table of Content
1. [Introduction](https://github.com/marthly/Real-Time-Scheduling-Simulator/new/master?readme=1#introduction)
2. [How To Run File](https://github.com/marthly/Real-Time-Scheduling-Simulator/new/master?readme=1#real-time-scheduling-simulator)
    - [Program Command Format](https://github.com/marthly/Real-Time-Scheduling-Simulator/new/master?readme=1#program-command-format)    
      - [sim - Simulate Scheduling Algorigthm](https://github.com/marthly/Real-Time-Scheduling-Simulator/new/master?readme=1#simulate-scheduling-algorithm)
      - [gen - Generate Input File](https://github.com/marthly/Real-Time-Scheduling-Simulator/new/master?readme=1#generate-input-file)
3. [Input File Format](https://github.com/marthly/Real-Time-Scheduling-Simulator/new/master?readme=1#input-file-format)

## Introduction
Real Time Scheduling Simulator that uses one of the 3 Scheduling Algorithm: 
- Rate Monotonic (RM)
- Earliest Deadline First (EDF)
- Deadlne Monotonic (DM)

to test whether a batch of tasks/jobs from a file will be completed before their deadline.<br /><br />

## How To Run File
1. Open terminal and navigate to project folder
2. In terminal execute: ***make RTS_Simulator*** (Compile all the files)
3. In terminal execute: ***./RTS_Simulator*** (Executes the program)
4. Enter and run one of the program command listed below

### Program Command Format
#### Simulate Scheduling Algorithm
- *sim [view] [algorithm] < [input_file]*

   **[view]** - Displays different views.
    - -d => Gives the user a detailed view of the simulated information.
    - -v => Gives the user a verbose view of the simulated information.
    - Without this parameter the user gets a minimal view.
  
   **[algorithm]** - Allows the user to execute the input file using different algorithm.
    - -a RM => Rate Monotonic Algorithm.
    - -a EDF => Earliest Deadline First Algorithm.
    - -a DM => Deadline Monotonic Algorithm.
    - User **MUST** select an algorithm.
    
   **[input_file]** - Allows the user to specify the name of the input file which is stored in the *Process_Files* folder.

  Example of sim command: ***$ sim -d -a RM < file1.txt***
  
  <br />
#### Generate Input File
- *gen [number_of_process] [mode] > [output_file]*

   **[number_of_process]** - Allows the user to generate a specific number of processes; **greater than 5**.
    - -n <number_of_process> => The number-of-process **must be 5 or greater.**
    - Without this parameter the program generates a random number of processes **between 5 and 10**. <br />
    ***NB. The upper bound can be changed in the FileGeneration.h file by changing the value of MAX_PROCESSES.***
   
   **[mode]** - Changes the version of input file that will be generated.
    - -d => The context switch is equal to 5 and the arrival times are different. 
    - Without this parameter thecontext switch is equal to 0 and the arrival time for all processes becomes 0.<br /><br />

## Input File Format
number_of_processes process_switch<br />
process_number arrival_time burst_time relative_deadline period<br />
process_number arrival_time burst_time relative_deadline period<br />
.<br />
.<br />
.<br />

#### Example: 
4 0<br />
1 0 2 16 10<br />
2 0 1 8 5<br />
3 0 5 16 30<br />
4 0 2 20 15<br />

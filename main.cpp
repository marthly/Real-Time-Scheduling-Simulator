#include "Execution.h"
#include <iostream>

using namespace std;

int main(){	
	while(1){
		string input = "";
		Execution exe;

		cout << "$ ";
		getline(cin,input);
		if(input != ""){
			exe.execute(input);	
		}
		else{
			cout << endl << "!!!!!!!!!!!!!!!!!!!!!" << endl
        				 << "!! Nothing Entered !!" << endl
       					 << "!!!!!!!!!!!!!!!!!!!!!" << endl << endl; 
		}		
	}	
}
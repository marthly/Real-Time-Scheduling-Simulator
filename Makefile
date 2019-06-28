RTS_Simulator: main.o Command.o FileParser.o Process.o Execution.o FileGeneration.o
	g++ main.o Command.o FileParser.o Process.o Execution.o FileGeneration.o -o RTS_Simulator

main.o: main.cpp
	g++ -c main.cpp

Command.o: Command.cpp Command.h
	g++ -c Command.cpp

FileParser.o: FileParser.cpp FileParser.h
	g++ -c FileParser.cpp

Process.o: Process.cpp Process.h
	g++ -c Process.cpp

Execution.o: Execution.cpp Execution.h
	g++ -c Execution.cpp

FileGeneration.o: FileGeneration.cpp FileGeneration.h
	g++ -c FileGeneration.cpp

clean:
	rm *.o RTS_Simulator

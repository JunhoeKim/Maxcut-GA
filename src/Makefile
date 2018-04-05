main_exe : main.o Chromosomes.o Graph.o Utils.o
	g++ -o main_exe main.o Chromosomes.o Graph.o Utils.o

Graph.o : Graph.cpp
	g++ -c -o Graph.o Graph.cpp

main.o : main.cpp
	g++ -c -o main.o main.cpp

Chromosomes.o : Chromosomes.cpp
	g++ -c -o Chromosomes.o Chromosomes.cpp

Utils.o : Utils.cpp
	g++ -c -o Utils.o Utils.cpp

clean :
	rm *.o main_exe
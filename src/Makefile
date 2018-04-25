CXX = g++
TARGET = main_exe
CXXFLAGS = -W -Wall -g -std=c++11 -O3
OBJECTS = main.o Chromosome.o Graph.o Utils.o Config.o GeneticSpace.o

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	rm *.o $(TARGET)

run :
	./$(TARGET)

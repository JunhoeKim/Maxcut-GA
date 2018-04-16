CC = g++
TARGET = main_exe
CFLAGS = -W -Wall -g
OBJECTS = main.o GeneticSpace.o Graph.o Utils.o Config.o Chromosome.o

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm *.o $(TARGET)

run :
	./$(TARGET)
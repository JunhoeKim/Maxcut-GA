CC = g++
TARGET = main_exe
CFLAGS = -W -Wall -g
OBJECTS = main.o Chromosomes.o Graph.o Utils.o

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm *.o $(TARGET)

run :
	./$(TARGET)
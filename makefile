.PHONY: all, clean

CC      = g++
CFLAGS  = -std=c++17 -Wall

TARGET = NetwrixTest

OBJFILES = \
	main.o \
	Request/Request.o \
	ThreadsController/ThreadsController.o \
	ThreadSearcher/ThreadSearcher.o \
	BankOfTasks/BankOfTasks.o \
	ThreadTask/ThreadTask.o \
	TaskExecutor/TaskExecutor.o \
	TaskExecutor/TaskExecutorDirectoryProcessing.o \
	TaskExecutor/TaskExecutorFileProcessing.o \
	TaskExecutor/FirstStepSearchInFile/FirstStepSearchInFile.o \
	TaskExecutor/SecondStepSearchInFile/SecondStepSearchInFile.o \
	TaskExecutor/CommonTypes/CommonTypes.o \
	FileSystem/FileSystem.o 

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $@ $(OBJFILES) -pthread

%.o : %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJFILES)




	
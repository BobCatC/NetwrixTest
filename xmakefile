.PHONY: all, clean

CC      = g++
CFLAGS  = -std=c++17 -Wall -I$(BOOST_INCLUDE_DIR)

TARGET = NetwrixTest
BOOST_INCLUDE_DIR = ./
BOOST_LIB_DIR = boost_lib
BOOST_LIB_NAMES = \
	$(BOOST_LIB_DIR)/libboost_filesystem.dylib \
	$(BOOST_LIB_DIR)/libboost_system.dylib \
	$(BOOST_LIB_DIR)/libboost_unit_test_framework.dylib 

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
	$(CC) -o $@ $(OBJFILES) $(BOOST_LIB_NAMES) -pthread

%.o : %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJFILES)




	
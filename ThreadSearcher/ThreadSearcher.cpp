//
//  ThreadSearcher.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <vector>
#include <iostream>

#include "ThreadSearcher.hpp"
#include "../TaskExecutor/TaskExecutor.hpp"


void printThreadError(const unsigned int threadID, const std::string& err);

void threadSearcher(BankOfTasks& tasksBank,
					const unsigned int threadID,
					const size_t cbMaxBufSize,
					const std::string& outputFileeDirectory,
					const std::string& thisThreadOutputFileName,
					const std::string& patternFileName,
					const std::regex& regexMask) {
	
	// Cycle:
	// 1) init executor
	//
	// 2) get vector (tasks) of new tasks from Bank (tasksBank)
	// 3) if size of tasks == 0, then go into cycle of checking for new tasks and end of work
	// 4) do task in executor and get vectors of new files and directories to check
	// 5) go to point (2)
	
	
	// this try-catch block catches only fatal errors
	try
	{
		std::vector<ThreadTask> newTasksFiles, newTasksDirectories;
		std::vector<ThreadTask> tasks;
		
		TaskExecutor executor(threadID, thisThreadOutputFileName, cbMaxBufSize, outputFileeDirectory, patternFileName, regexMask);
		
		// because of allocating memory and opening files, "init()" was removed from constructor
		executor.init();
		
		while(true) {
			
			tasks = tasksBank.getVectorOfTasks(threadID);
			
			if(tasks.empty()) {
				
				// isAllWorkDonw returns true only when all threads get here
				// ( to prevent the situation when one thread in the start takes all the tasks and another threads end work because of no tasks )
				if(tasksBank.isAllWorkDone()) {
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}
			
			
			newTasksFiles.clear();
			newTasksDirectories.clear();
			
			for(const auto& task : tasks) {
				
				executor.doTask(task, newTasksFiles, newTasksDirectories);
			}
			
			tasksBank.appendTasks(newTasksFiles, newTasksDirectories);
			
		}
	
	} catch(const std::string& err) {
		printThreadError(threadID, err);
		tasksBank.fatalErrorHappened();
	} catch(const char* s) {
		printThreadError(threadID, std::string(s));
		tasksBank.fatalErrorHappened();
	} catch(const std::exception& e) {
		printThreadError(threadID, e.what());
		tasksBank.fatalErrorHappened();
	}
	
	
	
	return;
}

void printThreadError(const unsigned int threadID, const std::string& err)
{
	std::cout << "! ! ! Thread " << threadID << " Error" << std::endl << err << std::endl;
}













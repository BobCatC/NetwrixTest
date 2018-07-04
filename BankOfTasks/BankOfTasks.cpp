//
//  BankOfTasks.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include "BankOfTasks.hpp"

#include <algorithm>
#include <iostream>


/* ---------------------------------------- BankOfTasks Constructor */

BankOfTasks::BankOfTasks(const unsigned int numberOfThreads, const Request& request):
_numberOfThreads(numberOfThreads)
{
	_threadHasNotWork = std::vector<bool>(_numberOfThreads, false);
	
	initFirstTask(request);
}


/* ---------------------------------------- BankOfTasks  */

void BankOfTasks::initFirstTask(const Request& request)
{
	_allTasksDirectories.emplace_back(request.startDirectory);
}


/* ---------------------------------------- BankOfTasks Thread Reports That Fatal Error Happened */

void BankOfTasks::fatalErrorHappened()
{
	waitForFlag();
	
	// after one thread failed, bank will return Zero tasks to others and isAllWorkDone will return true
	_fatalError = true;
	
	_flag.clear();
}


/* ---------------------------------------- BankOfTasks isAllWorkDone Return True If Any Thread Reported About FatalError Or If All Threads Don't Have Tasks */

bool BankOfTasks::isAllWorkDone()
{
	bool result;
	
	waitForFlag();
	
	// is equal to true
	const auto op = std::bind(std::equal_to<bool>(), std::placeholders::_1, true);
	bool allThreadsWIthoutWork = std::all_of(_threadHasNotWork.begin(), _threadHasNotWork.end(), op);
	
	result = ( _fatalError || allThreadsWIthoutWork );
	
	_flag.clear();

	return result;
}


/* ---------------------------------------- BankOfTasks getVectorOfTasks Returns Vector Of Tasks To Do. -------------------------- */
/* ---------------------------------------- If Bank Has Tasks With Files, Firstly It Return Tasks With Files (No Tiwh Directories) */

std::vector<ThreadTask> BankOfTasks::getVectorOfTasks(const unsigned int threadID)
{
	waitForFlag();
	
	std::vector<ThreadTask> tasks;
	
	if(!_fatalError) {
		// Firstly we process files
		// And if threre is no task whith file, we return tasks whith directories
		std::vector<ThreadTask>& tasksBank = _allTasksFiles.empty() ? _allTasksDirectories : _allTasksFiles;
		
		size_t maxTasksToGive = 1;
		
		// If we return tasks with files, we can return Many tasks
		if(!_allTasksFiles.empty()) {
			maxTasksToGive = 80;
		}
		
		if(!tasksBank.empty()) {
			
			markThreadAsWorkingOne(threadID);
			
			fillNewTasks(tasks, tasksBank, maxTasksToGive);
		}
		else {
			markThreadAsWithoutWorkOne(threadID);
		}
	}
	
	_flag.clear();
	return tasks;
}


/* ---------------------------------------- BankOfTasks fillNewTasks Fills Given To It Vector Of New Tasks */

void BankOfTasks::fillNewTasks(std::vector<ThreadTask>& tasksResult, std::vector<ThreadTask>& tasksBank, size_t maxTasksToGive)
{
	const size_t crtTasksCount = tasksBank.size();
	
	// rounding up
	const size_t tasksForEach = (crtTasksCount + _numberOfThreads - 1) / _numberOfThreads;
	size_t tasksToGive = std::min(maxTasksToGive, tasksForEach);
	
	
	for(int i = 0; i < tasksToGive; ++i) {
		// take the last one
		tasksResult.emplace_back(tasksBank[crtTasksCount - 1 - i]);

		tasksBank.pop_back();
	}
}


/* ---------------------------------------- BankOfTasks markThreadAsWorkingOne */

void BankOfTasks::markThreadAsWorkingOne(unsigned int threadID)
{
	_threadHasNotWork[threadID] = false;
}


/* ---------------------------------------- BankOfTasks markThreadAsWithoutWorkOne */

void BankOfTasks::markThreadAsWithoutWorkOne(unsigned int threadID)
{
	_threadHasNotWork[threadID] = true;
}


/* ---------------------------------------- BankOfTasks waitForFlag Sets The Flag To Synchronize Threads */

void BankOfTasks::waitForFlag()
{
	while (_flag.test_and_set()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}


/* ---------------------------------------- BankOfTasks appendTasks Fills Interior Vectors Of Tasks */

void BankOfTasks::appendTasks(const std::vector<ThreadTask> &newTasksFiles, const std::vector<ThreadTask>& newTasksDirectories)
{
	waitForFlag();
	
	_allTasksFiles.insert(_allTasksFiles.end(), newTasksFiles.begin(), newTasksFiles.end());
	_allTasksDirectories.insert(_allTasksDirectories.end(), newTasksDirectories.begin(), newTasksDirectories.end());
	
	_flag.clear();
	
}


/* ---------------------------------------- BankOfTasks log */

void BankOfTasks::log(const std::string &str)
{
	waitForFlag();
	
	std::cout << "NetwrixTest: " << str << std::endl;
	
	_flag.clear();
}








//
//  BankOfTasks.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "BankOfTasks.hpp"

BankOfTasks::BankOfTasks(const unsigned int numberOfThreads, const Request& request):
_numberOfThreads(numberOfThreads),
_allTasksFiles("All Tasks Files"),
_allTasksDirectories("All Tasks Directories")
{
	
	_numberOfThreadsWithoutWork = 0;
	_threadsWithoutWork = std::vector<bool>(_numberOfThreads, false);
	
	initFirstTask(request);
}

void BankOfTasks::initFirstTask(const Request& request) {
//	const ThreadTask firstTask(request.startDirectory);
	_allTasksDirectories.push_back(request.startDirectory);
}


bool BankOfTasks::isAllWorkDone() {
	bool result;
	
	waitForFlag();

	result = ( _numberOfThreadsWithoutWork == _numberOfThreads );
	_flag.clear();

	return result;
}

std::vector<ThreadTask> BankOfTasks::getVectorOfTasks(const unsigned int threadID) {
	
	waitForFlag();
	
	std::vector<ThreadTask> tasks;
	const size_t maxTasksToGive = 20;
	
	MyVector<const std::string>& tasksBank = _allTasksFiles.empty() ? _allTasksDirectories : _allTasksFiles;
	
	
	
	if(!tasksBank.empty()) {
		
		if(_threadsWithoutWork[threadID] == true) {
			_threadsWithoutWork[threadID] = false;
			--_numberOfThreadsWithoutWork;
		}
		
		const size_t crtTasksCount = tasksBank.size();
		const size_t tasksForEach = (crtTasksCount + _numberOfThreads - 1) / _numberOfThreads;
		size_t tasksToGive = maxTasksToGive < tasksForEach ? maxTasksToGive : tasksForEach;
		
//		tasks = std::vector<ThreadTask>(_allTasks.end() - tasksToGive, _allTasks.end());
		
//		tasks.reserve(tasksToGive);
		
		for(int i = 0; i < tasksToGive; ++i) {
			if(tasksBank[crtTasksCount - 1 - i].empty() || tasksBank[crtTasksCount - 1 - i][0] == '\0') {
				int a = 0;
			}
			tasks.push_back(ThreadTask(bfs::path(tasksBank[crtTasksCount - 1 - i])));
			tasksBank.pop_back();
		}
		
		
		
//		_allTasks.resize(_allTasks.size() - tasksToGive);
	}
	else {
		
		if(_threadsWithoutWork[threadID] == false) {
			_threadsWithoutWork[threadID] = true;
			++_numberOfThreadsWithoutWork;
		}
	}
	
	_flag.clear();

	return tasks;
}

void BankOfTasks::waitForFlag() {
	
	while (_flag.test_and_set()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


void BankOfTasks::appendTasks(const MyVector<ThreadTask> &newTasksFiles, const MyVector<ThreadTask>& newTasksDirectories) {
	bool f = true;
	if(!newTasksFiles.empty()) {
		
		for(const auto& task : newTasksFiles.vec()) {
			if(true)
				_allTasksFiles.push_back(task.getFilePath());
			f = !f;
		}
	}
	
	if(!newTasksDirectories.empty()) {
		
		for(const auto& task : newTasksDirectories.vec()) {
			if(true)
				_allTasksDirectories.push_back(task.getFilePath());
			f = !f;
		}
	}
	
//	_allTasks.insert(_allTasks.end(), newTasks.begin(), newTasks.end());
	
}










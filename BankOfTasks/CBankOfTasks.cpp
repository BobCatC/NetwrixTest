//
//  CBankOfTasks.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "CBankOfTasks.hpp"
#ifdef __APPLE__
#include <unistd.h>
#else
#include<Windows.h>
#define sleep(ms) Sleep(ms)
#endif

CBankOfTasks::CBankOfTasks(const unsigned int numberOfThreads):
_numberOfThreads(numberOfThreads)
{
	_numberOfThreadsWithoutWork = 0;
	_threadsWithoutWork = std::vector<bool>(_numberOfThreads, false);
}

bool CBankOfTasks::workDone() {
	bool result;
	
	while (_flag.test_and_set()) {
		sleep(1);
	}
	
	result = ( _numberOfThreadsWithoutWork == _numberOfThreads );
	_flag.clear();
	
	return result;
}

const std::vector<CThreadTask> CBankOfTasks::getVectorOfTasks(const unsigned int threadID) {
	std::vector<CThreadTask> tasks;
	const size_t maxTasksToGive = 20;
	
	while (_flag.test_and_set()) {
		sleep(1);
	}
	
	if(_allTasks.size() != 0) {
		
		if(_threadsWithoutWork[threadID] == true) {
			_threadsWithoutWork[threadID] = false;
			--_numberOfThreadsWithoutWork;
		}
		
		size_t tasksToGive = maxTasksToGive < _allTasks.size() ? maxTasksToGive : _allTasks.size();
		tasks = std::vector<CThreadTask>(_allTasks.end() - tasksToGive, _allTasks.end());
		_allTasks.resize(_allTasks.size() - tasksToGive);
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

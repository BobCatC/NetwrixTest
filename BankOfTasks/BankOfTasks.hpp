//
//  BankOfTasks.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef BankOfTasks_hpp
#define BankOfTasks_hpp

#include <vector>
#include <atomic>
#include <thread>

#include "../ThreadTask/ThreadTask.hpp"
#include "../request/Request.hpp"
#include "../PathString.hpp"

class BankOfTasks {
	
public:
	
	BankOfTasks(const unsigned int numberOfThreads, const Request& request);
	BankOfTasks(const BankOfTasks& other) = delete;
	BankOfTasks(BankOfTasks& other) = delete;
	
	
	std::vector<ThreadTask> getVectorOfTasks(const unsigned int threadID);
	void appendTasks(const std::vector<ThreadTask>& newTasksFiles, const std::vector<ThreadTask>& newTasksDirectories);
	
	bool isAllWorkDone();
	
	
private:
	
	const unsigned int _numberOfThreads;
	
	std::vector<PathString> _allTasksFiles, _allTasksDirectories;
	
	unsigned int _numberOfThreadsWithoutWork;
	std::vector<bool> _threadsWithoutWork;
	
	std::atomic_flag _flag = ATOMIC_FLAG_INIT;

	
	void initFirstTask(const Request& request);
	
	void waitForFlag();
};

#endif /* BankOfTasks_hpp */

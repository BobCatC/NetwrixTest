//
//  BankOfTasks.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef BankOfTasks_hpp
#define BankOfTasks_hpp

#include <vector>
#include <atomic>
#include <thread>

#include "../ThreadTask/ThreadTask.hpp"
#include "../Request/Request.hpp"

class BankOfTasks {
	
public:
	
	BankOfTasks(const unsigned int numberOfThreads, const Request& request);
	BankOfTasks(const BankOfTasks& other) = delete;
	BankOfTasks(BankOfTasks& other) = delete;
	
	
	std::vector<ThreadTask> getVectorOfTasks(const unsigned int threadID);
	void appendTasks(const std::vector<std::string>& newTasksFiles, const std::vector<std::string>& newTasksDirectories);
	
	bool isAllWorkDone();
	
	void fatalErrorHappened();
	
private:
	
	std::atomic_flag _flag = ATOMIC_FLAG_INIT;
	const unsigned int _numberOfThreads;
	
	bool _fatalError = false;
	
	std::vector<std::string> _allTasksFiles, _allTasksDirectories;
	
	unsigned int _numberOfThreadsWithoutWork;
	std::vector<bool> _threadHasNotWork;
	
	
	void fillNewTasks(std::vector<ThreadTask>& tasksResult, std::vector<std::string>& tasksBank, size_t maxTasksToGive);

	void markThreadAsWithoutWorkOne(unsigned int threadID);
	void markThreadAsWorkingOne(unsigned int threadID);
	
	void initFirstTask(const Request& request);
	
	void waitForFlag();
};

#endif /* BankOfTasks_hpp */

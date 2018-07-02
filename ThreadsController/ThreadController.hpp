//
//  ThreadsController.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef CThreadsController_hpp
#define CThreadsController_hpp

#include <thread>
#include <atomic>
#include <vector>

#include "../BankOfTasks/BankOfTasks.hpp"
#include "../Request/Request.hpp"


class ThreadsController {
	
public:
	
	ThreadsController(const Request& request, const unsigned int numberOfThreads = 8);
	ThreadsController(const ThreadsController& other) = delete;
	ThreadsController(ThreadsController& other) = delete;
	
	void createThreads();
	
	~ThreadsController();
	
private:
	
	const Request& _request;
	const size_t _cbMaxBufSizeForProgramm = (5 * 1024 * 1024);
	
	// main output file, which name was got from command line
	FILE* _outputFile = nullptr;
	char* _buf = nullptr;
	
	// the directory of "_thisThreadOutputFile"
	std::string _outputFileDirectory;
	
	const unsigned int _numberOfThreads;
	std::vector<std::thread> _threads;
	
	// each thread has each own output file (synchronization would take too much time)
	std::vector<std::string> _threadsOutputFilesNames;
	
	// each thread has a link to synchronized bank
	// each thread asks for tasks
	// each thread return new tasks (if got it)
	BankOfTasks _tasksBank;
	
	std::regex regexMask;
	
	
	
	void openOutputFile();
	
	void initThreadsOutputFilesNames();
	
	void initRegexMask();

	void startThreads();
	void waitThreads();
	
	void mergeOutput();
	void getOutputOfThread(size_t threadID);
	void moveOutputOfThread(FILE* threadOutputFile);
};

#endif /* CThreadsController_hpp */

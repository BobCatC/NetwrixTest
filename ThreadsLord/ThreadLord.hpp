//
//  ThreadLord.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef CThreadLord_hpp
#define CThreadLord_hpp

#include <thread>
#include <atomic>
#include <vector>

#include "../BankOfTasks/BankOfTasks.hpp"
#include "../Request/Request.hpp"


class ThreadLord {
	
public:
	
	ThreadLord(const Request& request, const unsigned int numberOfThreads = 8);
	ThreadLord(const ThreadLord& other) = delete;
	ThreadLord(ThreadLord& other) = delete;
	
	~ThreadLord();
	
private:
	
	const Request& _request;
	const size_t _cbMaxBufSizeForProgramm = 1024 * 1024 * 8;
	
	// main output file, which name was got from command line
	FILE* _outputFile = nullptr;
	char* _buf = nullptr;
	
	std::string _OutputFileDirectory;
	
	const unsigned int _numberOfThreads;
	std::vector<std::thread> _threads;
	
	// each thread has a link to synchronized bank
	// each thread asks for tasks
	// each thread return new tasks (if got it)
	BankOfTasks _tasks;
	
	std::regex regexMask;
	
	
	
	void openOutputFile();
	
	void initRegexMask();

	void startThreads();
	void waitThreads();
	
	void collectOutput();
	void getOutputOfThread(size_t threadID);
	void moveOutputOfThread(FILE* threadOutputFile);
};

#endif /* CThreadLord_hpp */

//
//  ThreadLord.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CThreadLord_hpp
#define CThreadLord_hpp

#include <thread>
#include <atomic>
#include <vector>

#include "../BankOfTasks/BankOfTasks.hpp"
#include "../Thread/ThreadSearcher.hpp"
#include "../request/Request.hpp"
#include "../PathString.hpp"


class ThreadLord {
	
public:
	
	ThreadLord(const Request& request, const unsigned int numberOfThreads = 8);
	ThreadLord(const ThreadLord& other) = delete;
	ThreadLord(ThreadLord& other) = delete;
	
private:
	
	const unsigned int _numberOfThreads;
	std::vector<std::thread> _threads;
	
	BankOfTasks _tasks;
	
	std::regex regexMask;
	
	
	void initRegexMask(const std::string& mask);

	void startThreads(const Request& request);
	void waitThreads();
	
};

#endif /* CThreadLord_hpp */

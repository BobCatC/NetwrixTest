//
//  ThreadLord.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "ThreadLord.hpp"

#include <iostream>

ThreadLord::ThreadLord(const Request& request, const unsigned int numberOfThreads):
_numberOfThreads(numberOfThreads),
_tasks(numberOfThreads, request)
{
	initRegexMask(request.mask);
	
	startThreads(request);

	waitThreads();
	
}

void ThreadLord::initRegexMask(const std::string& mask)
{
	std::string result;
	for(size_t i = 0; i < mask.size(); ++i) {
		switch (mask[i]) {
			case '*':
				result.append(".*");
				break;
			case '?':
				result.append(".");
				break;
			case '.':
				result.append("[.]");
				break;
			default:
				result.push_back(mask[i]);
				break;
		}
	}
	regexMask = std::regex(result);
}


void ThreadLord::startThreads(const Request& request)
{
	
	const unsigned int maxNumberOfThreads = 100;
	if(_numberOfThreads == 0 || _numberOfThreads > maxNumberOfThreads) {
		throw "Wrong Number Of Threads";
	}
	
	
	size_t cbBufSize = (4 * 1024 * 1024) / _numberOfThreads;
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		
		_threads.push_back(std::thread(threadSearcher, std::ref(_tasks), iThread, cbBufSize, request.patternFileName, std::ref(regexMask)));
		
	}
}

void ThreadLord::waitThreads()
{
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		_threads[iThread].join();
	}
	
}

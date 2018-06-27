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

void ThreadLord::initRegexMask(const RegexString& mask) {
	RegexString result;
	for(size_t i = 0; i < mask.size(); ++i) {
		switch (mask[i]) {
			case '*':
				result.append(getPathStringFromCString(".*"));
				break;
			case '?':
				result.append(getPathStringFromCString("."));
				break;
			case '.':
				result.append(getPathStringFromCString("\."));
				break;
			default:
				result.push_back(mask[i]);
				break;
		}
	}
	regexMask = regex(result);
}


void ThreadLord::startThreads(const Request& request) {
	
	const unsigned int maxNumberOfThreads = 100;
	if(_numberOfThreads == 0 || _numberOfThreads > maxNumberOfThreads) {
		throw "Wrong Number Of Threads";
	}
	
	
	size_t cbBufSize = (1024 * 400) * _numberOfThreads;
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		
		_threads.push_back(std::thread(threadSearcher, std::ref(_tasks), iThread, cbBufSize, request.patternFileName, std::ref(regexMask)));
		
	}
}

void ThreadLord::waitThreads() {
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		_threads[iThread].join();
	}
	
}

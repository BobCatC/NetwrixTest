//
//  CThreadLord.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "CThreadLord.hpp"

CThreadLord::CThreadLord(const CRequest& request, const unsigned int numberOfThreads):
_numberOfThreads(numberOfThreads)
{
	__log("Creating threads");
	startThreads();
	__log("Creating threads done");
	waitThreads();
	__log("All threads ended");
}

void CThreadLord::startThreads() {
	const unsigned int maxNumberOfThreads = 100;
	if(_numberOfThreads == 0 || _numberOfThreads > maxNumberOfThreads) {
		throw "Wrong Number Of Threads";
	}
	
	CBankOfTasks tasks(_numberOfThreads);
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		_threads.push_back(std::thread(threadSearcher, std::ref(tasks), iThread));
	}
}

void CThreadLord::waitThreads() {
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		_threads[iThread].join();
	}
}

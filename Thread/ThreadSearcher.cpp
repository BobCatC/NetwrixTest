//
//  ThreadSearcher.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "ThreadSearcher.hpp"
#include <vector>
#ifdef __APPLE__
#include <unistd.h>
#else
#include<Windows.h>
#define sleep(ms) Sleep(ms)
#endif
#include <atomic>

void threadSearcher(CBankOfTasks& tasksBank, const unsigned int threadID) {
	
	while(true) {
		
		const std::vector<CThreadTask> tasks = tasksBank.getVectorOfTasks(threadID);
		
		if(tasks.size() == 0) {
			if(tasksBank.workDone()) {
				break;
			}
			sleep(10);
			continue;
		}
		
		for(size_t iTask = 0; iTask < tasks.size(); ++iTask) {
			
		}
		
	}
	
	return;
}

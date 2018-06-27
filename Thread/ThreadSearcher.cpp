//
//  ThreadSearcher.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "ThreadSearcher.hpp"



extern size_t numberOfFiles, fileDoneSize;

void threadSearcher(BankOfTasks& tasksBank, const unsigned int threadID, const size_t cbMaxBufSize, const PathString& patternFileName, const regex& regexMask) {
	
	std::vector<ThreadTask> newTasksFiles, newTasksDirectories;
	
	std::vector<ThreadTask> tasks;
	
	std::ofstream fout("output.txt");
	
	std::vector<std::string> result;
	
	std::vector<std::vector<std::set<int32_t>>> entries;
	
	size_t numberOfDoneTasks = 0;
	
	
	char* buf = new char[cbMaxBufSize];
	
	
	while(true) {
		
		tasks = tasksBank.getVectorOfTasks(threadID);
		
		if(tasks.size() == 0) {
			
			if(tasksBank.isAllWorkDone()) {
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		numberOfDoneTasks += tasks.size();
		
		newTasksFiles.clear();
		newTasksDirectories.clear();
		
		for(size_t iTask = 0; iTask < tasks.size(); ++iTask) {
			
//			std::cout << "\t" << threadID << "\t" << tasks[iTask].getFileName() << std::endl;
			
//			if(std::regex_match(tasks[iTask].getFileName(), std::regex(".*README.*"))) {
//				int i = 0;
//			}
			
			result.clear();
			
			
			tasks[iTask].doTask(cbMaxBufSize, buf, newTasksFiles, newTasksDirectories, patternFileName, regexMask, result, entries);
			
//			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			
			if(!result.empty()) {
				
				fout << "*** In File " << tasks[iTask].getFileName() << std::endl;
				for(const auto& str: result) {
					fout << "\t" << str;
				}
			}

			
			
		}
		
		
		tasksBank.appendTasks(newTasksFiles, newTasksDirectories);
		
		
		
	}
	
	delete [] buf;
	
	fout << "*** DONE: " << numberOfDoneTasks << std::endl;
	fout << "*** FILES NUMBER: " << numberOfFiles << std::endl;
	fout << "*** SIZE OF FILES: " << fileDoneSize << std::endl;
	
	return;
}















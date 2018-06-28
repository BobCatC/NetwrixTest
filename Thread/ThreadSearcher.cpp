//
//  ThreadSearcher.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//
#define NO_MEM_TRACK

#include "ThreadSearcher.hpp"
//#include "../MemTrack.hpp"


extern size_t numberOfFiles, fileDoneSize;

void threadSearcher(BankOfTasks& tasksBank, const unsigned int threadID, const size_t cbMaxBufSize, const std::string& patternFileName, const std::regex& regexMask) {
	
	std::vector<std::string> newTasksFiles, newTasksDirectories;
	std::vector<ThreadTask> tasks;
	
	std::ofstream fout("output_" + std::to_string(threadID) + ".txt");
	
	size_t numberOfDoneTasks = 0;
	int c = 0;
	
	try
	{
		TaskExecutor executor(threadID, cbMaxBufSize, patternFileName, regexMask);
		
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
				
				const std::vector<FirstFragmentEntry>& result = executor.doTask(tasks[iTask], newTasksFiles, newTasksDirectories);
				
				if(!result.empty()) {
					
					fout << "*** In File " << tasks[iTask].getFilePath()<< std::endl;
					
					fout << "\tNumber Of Entries : " << result.size() << std::endl;
					
					for(const auto& position: result) {
						fout << "\tposition : " << position << std::endl;
					}
				}
			}
			
//			if(c == 0) {
//				c = 1000;
////				MemTrack::TrackListMemoryUsage();
//				
//			}
//			--c;
//			
			tasksBank.appendTasks(newTasksFiles, newTasksDirectories);
			
		}
		
		fout << "*** DONE: " << numberOfDoneTasks << std::endl;
		fout << "*** FILES NUMBER: " << executor.doneFiles << std::endl;
		fout << "*** SIZE OF FILES: " << executor.doneSize << std::endl;
		std::cout << "Prefix function time" << executor.allTimePrefix / (1000 * 1000) << std::endl;
	}
	catch(const std::string& err)
	{
		std::cout << "! ! ! Thread " << threadID << " Error" << std::endl << err << std::endl;
		return;
	}
	
	
	
	return;
}















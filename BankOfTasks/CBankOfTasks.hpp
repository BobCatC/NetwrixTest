//
//  CBankOfTasks.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CBankOfTasks_hpp
#define CBankOfTasks_hpp

#include <vector>

#include "../ThreadTask/CThreadTask.hpp"

class CBankOfTasks {
	
public:
	
	CBankOfTasks(const unsigned int numberOfThreads);
//	CBankOfTasks(const CBankOfTasks& other) = delete;
//	CBankOfTasks(CBankOfTasks& other) = delete;
	
	const std::vector<CThreadTask> getVectorOfTasks(const unsigned int threadID);
	bool workDone();
	
	void appendTasks(const std::vector<CThreadTask>& newTasks);
	
private:
	
	std::atomic_flag _flag = ATOMIC_FLAG_INIT;
	
	std::vector<CThreadTask> _allTasks;
	const unsigned int _numberOfThreads;
	unsigned int _numberOfThreadsWithoutWork;
	std::vector<bool> _threadsWithoutWork;
	
};

#endif /* CBankOfTasks_hpp */

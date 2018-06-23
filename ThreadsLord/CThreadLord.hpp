//
//  CThreadLord.hpp
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

#include "../BankOfTasks/CBankOfTasks.hpp"
#include "../Thread/ThreadSearcher.hpp"
#include "../request/CRequest.hpp"

class CThreadLord {
	
public:
	
	CThreadLord(const CRequest& request, const unsigned int numberOfThreads = 8);
	CThreadLord(const CThreadLord& other) = delete;
	CThreadLord(CThreadLord& other) = delete;
	
private:
	
	std::vector<std::thread, std::allocator<std::thread>> _threads;
	const unsigned int _numberOfThreads;
	
	
	void startThreads();
	void waitThreads();
};

#endif /* CThreadLord_hpp */

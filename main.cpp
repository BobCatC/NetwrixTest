//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include <iostream>
#include "request/Request.hpp"
#include "ThreadsLord/ThreadLord.hpp"
#include "MemoryKeeper.h"
#include "MyVector.hpp"

#include <stdio.h>

void specialThread(const Request& request, const unsigned int nThreads) {
	int* p = (int*)malloc(1024 * 1024 * 2);
	memset(p, 1, 1024 * 1024 * 2);
	ThreadLord(request, nThreads);
//	delete [] p;
	free(p);
}

int counterTest = 0;

class TestClass {
private:
	int a[24];
	int c;
public:
	
	TestClass() {
		c = counterTest++;
		std::cout << "Constructor " << c << std::endl;
	}
	~TestClass() {
		std::cout << "Destructor " << c << std::endl;
	}
};

int main(int argc, const char * argv[]) {
	// * -> .*
	// ? -> .
	// . -> \.
//	{
//	MyVector<std::vector<int>> v;
//
//	for(int i = 0; i < 1000; ++i) {
//
//		v.push_back(std::vector<int>(10000));
//
//	}
//		uint64_t m_totalMemory;
//		uint64_t total = 0;
//		size_t size = sizeof(total);
//		if( !sysctlbyname("hw.physmem", &total, &size, NULL, 0) )
//			m_totalMemory = total;
//		std::cout << total / (1024 * 1024) << " MB\n";
//
//
//	for(int i = 0; i < 1000; ++i) {
//		v.pop_back();
//	}
//
//		if( !sysctlbyname("hw.physmem", &total, &size, NULL, 0) )
//			m_totalMemory = total;
//		std::cout << total / (1024 * 1024) << " MB\n";
//
//	v.fit();
//
//	if( !sysctlbyname("hw.physmem", &total, &size, NULL, 0) )
//			m_totalMemory = total;
//		std::cout << total / (1024 * 1024) << " MB\n";
//
//	v.push_back(std::vector<int>());
//	}
//	uint64_t m_totalMemory;
//	uint64_t total = 0;
//	size_t size = sizeof(total);
//
//	if( !sysctlbyname("hw.physmem", &total, &size, NULL, 0) )
//		m_totalMemory = total;
//	std::cout << total / (1024 * 1024) << " MB\n";
//
//	return 0;
	
	
	try {
		
		// parsing arguments into CRequest
		std::cout << "Parsing" << std::endl;
		Request request(argc, argv);
		std::cout << "Parse done" << std::endl;
		
		
		const unsigned int nThreads = 1;
//		ThreadLord lord(request, nThreads);
		std::thread(specialThread, std::ref(request), nThreads).join();
		
	} catch (const std::string& s) {
		std::cout << s;
	}
	
	std::cout << std::endl << "DONE WAITING";
	std::this_thread::sleep_for(std::chrono::seconds(10));
	
	std::cout << std::endl << FILE_OPEN_COUNTER << std::endl << "max == " << MAX_OPENED << std::endl;
	
	return 0;
}








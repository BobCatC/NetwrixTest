//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include <iostream>
#include "request/CRequest.hpp"
#include "log/CLogTracker.hpp"
#include "ThreadsLord/CThreadLord.hpp"


int main(int argc, const char * argv[]) {
	
	try {
		
		// parsing arguments into CRequest
		CRequest request(argc, argv);
		
		
		const unsigned int nThreads = 8;
		CThreadLord lord(request, nThreads);
		
	} catch (const std::string& s) {
		std::cout << s;
	}
	
	__log("Program ended");
	return 0;
}

//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

//#include "MemTrack.hpp"

#include <iostream>

#include "request/Request.hpp"
#include "ThreadsLord/ThreadLord.hpp"
#include "PathString.hpp"


int main(int argc, const char * argv[]) {
	// * -> .*
	// ? -> .
	// . -> \.

	

	argc = 9;
	argv[1] = "-p"; argv[2] = "C:\\Users";
	argv[3] = "-i"; argv[4] = "copyright.txt";
	argv[5] = "-o"; argv[6] = "output.txt";
	argv[7] = "-m"; argv[8] = "*.*";

	
	const auto timeStart = std::chrono::system_clock::now();
	
	try {
		
		// parsing arguments into CRequest
		std::cout << "Parsing" << std::endl;
		Request request(argc, argv);
		std::cout << "Parse done" << std::endl;
		
		
		const unsigned int nThreads = 1;
		ThreadLord lord(request, nThreads);
		
	} catch (const std::string& s) {
		std::cout << s;
	}
	
	const auto timeEnd = std::chrono::system_clock::now();
	
	const int time = std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeStart).count();
	
	std::cout << std::endl << time << std::endl;
	
	return 0;
}








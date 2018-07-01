//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <iostream>

#include "Request/Request.hpp"
#include "ThreadsLord/ThreadLord.hpp"


int main(int argc, const char * argv[]) {

	
//	argc = 9;
//	argv[1] = "-p"; argv[2] = "C:\\Users";
//	argv[3] = "-i"; argv[4] = "copyright.txt";
//	argv[5] = "-o"; argv[6] = "output.txt";
//	argv[7] = "-m"; argv[8] = "*.*";
	
	try {
		const auto timeStart = std::chrono::system_clock::now();

		
		// parsing arguments into Request
		Request request;
		request.parseArguments(argc, argv);
		
		const unsigned int nThreads = 4;
		
		// creates threads, gives them a BankTasks ref
		// and waits for them to compile theire output files into one
		ThreadLord lord(request, nThreads);
		
		lord.createThreads();
		
		const auto timeEnd = std::chrono::system_clock::now();
		const int time = (int)std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
		
		std::cout << std::endl << time << std::endl;

	} catch (const std::string& s) {
		std::cout << s << std::endl;
	} catch (const char* s) {
		std::cout << s << std::endl;
	}
	
	
	
	return 0;
}






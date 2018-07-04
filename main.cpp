//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <iostream>

#include "Request/Request.hpp"
#include "ThreadsController/ThreadsController.hpp"


int main(int argc, const char * argv[]) {
	
	try {
		const auto timeStart = std::chrono::system_clock::now();

		
		// parsing arguments into Request
		Request request;
		request.parseArguments(argc, argv);
		
		const unsigned int nThreads = 4;
		
		// creates threads, gives them a BankTasks ref
		// and waits for them to compile theire output files into one
		ThreadsController controller(request, nThreads);
		
		controller.createThreads();
		
		const auto timeEnd = std::chrono::system_clock::now();
		const int time = (int)std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
		
		std::cout << "DONE" << std::endl << "Time : " << time << " ms" << std::endl;

	} catch (const std::string& s) {
		std::cout << s << std::endl;
	} catch (const char* s) {
		std::cout << s << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Unknown exception : " << e.what() << std::endl;
	}
	
	return 0;
}







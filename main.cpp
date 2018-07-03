//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <iostream>

#include "Request/Request.hpp"
#include "ThreadsController/ThreadController.hpp"
#include <vector>

int main(int argc, const char * argv[]) {

//	argc = 9;
//	argv[1] = "-p"; argv[2] = "boost";
//	argv[3] = "-i"; argv[4] = "Copyright.txt";
//	argv[5] = "-o"; argv[6] = "output.txt";
//	argv[7] = "-m"; argv[8] = "*.h*";
	
	
	try {
		const auto timeStart = std::chrono::system_clock::now();

		
		// parsing arguments into Request
		Request request;
		request.parseArguments(argc, argv);
		
		const unsigned int nThreads = 1;
		
		// creates threads, gives them a BankTasks ref
		// and waits for them to compile theire output files into one
		ThreadsController controller(request, nThreads);
		
		controller.createThreads();
		
		const auto timeEnd = std::chrono::system_clock::now();
		const int time = (int)std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
		
		std::cout << "Time : " << time << " ms" << std::endl;

	} catch (const std::string& s) {
		std::cout << s << std::endl;
	} catch (const char* s) {
		std::cout << s << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Unknown exception : " << e.what() << std::endl;
	}
	
	
	std::this_thread::sleep_for(std::chrono::seconds(3));
	return 0;
}







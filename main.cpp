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
#include "PathString.hpp"



int wmain(int argc, const PathChar * argv[]) {
	// * -> .*
	// ? -> .
	// . -> \.

	

	argc = 9;
	argv[1] = "-p"; argv[2] = "C:\\Users";
	argv[3] = "-i"; argv[4] = "copyright.txt";
	argv[5] = "-o"; argv[6] = "output.txt";
	argv[7] = "-m"; argv[8] = "*.*";
	
	try {
		
		// parsing arguments into CRequest
		cout << "Parsing" << std::endl;
		Request request(argc, argv);
		cout << "Parse done" << std::endl;
		
		
		const unsigned int nThreads = 1;
		ThreadLord lord(request, nThreads);
		
	} catch (const PathChar& s) {
		cout << s;
	}
	
	return 0;
}








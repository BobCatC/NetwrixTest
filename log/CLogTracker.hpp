//
//  CLogTracker.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CLogTracker_hpp
#define CLogTracker_hpp

#include <fstream>

#define STD_LOGFILE_NAME "log.txt"

class CLogTracker {
	
public:
	
	CLogTracker(const CLogTracker& other) = delete;
	std::ifstream extracted(const std::string &logFileName);
	
	CLogTracker(const std::string& logFileName = STD_LOGFILE_NAME, bool rewriteFile = true);
	
	void operator() (const std::string& logString);
	
	void turnOn();
	void turnOff();
	
private:
	std::ofstream _logFile;
	std::string _logFileName;
	bool writeInFile;
};

extern CLogTracker __log;

#endif /* CLogTracker_hpp */

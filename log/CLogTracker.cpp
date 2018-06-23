//
//  CLogTracker.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "CLogTracker.hpp"

CLogTracker __log;

CLogTracker::CLogTracker(const std::string& logFileName, bool rewriteFile): _logFileName(logFileName) {
	_logFile = std::ofstream(logFileName, rewriteFile? std::ofstream::out : std::ofstream::ate);
	writeInFile = true;
}

void  CLogTracker::operator() (const std::string& logString) {
	if(writeInFile) {
		if(_logFile) {
			_logFile << logString << std::endl;
		}
		else {
			throw ( "Couldn't open on write log file \'" + _logFileName + "\'" );
		}
	}
}

void CLogTracker::turnOn() {
	writeInFile = true;
}

void CLogTracker::turnOff() {
	writeInFile = false;
}

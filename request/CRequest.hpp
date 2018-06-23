//
//  CRequest.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CRequest_h
#define CRequest_h

#include <string>
#include "../log/CLogTracker.hpp"

class CRequest {
	
public:
	CRequest(const int argc, const char** argv);
	
	
	// getters
	const std::string& startDirectory = _startDirectory;
	const std::string& mask = _mask;
	const std::string& patternFileName = _patternFileName;
	const std::string& outputFileName = _outputFileName;
	const bool& nolog = _nolog;
	
private:
	std::string _startDirectory;
	std::string _mask;
	std::string _patternFileName;
	std::string _outputFileName;
	bool _nolog = false;
	
	void parseArguments(const int argc, const char** argv);
	void parsePairsOfArguments(const int argc, const char** argv);
	void checkForEmpty() const;
	
};

#endif /* CRequest_h */

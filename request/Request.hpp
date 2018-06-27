//
//  Request.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CRequest_h
#define CRequest_h

#include <string>

#include "../PathString.hpp"


class Request {
	
public:
	Request(const int argc, const PathChar** argv);
	
	
	// getters
	const PathString& startDirectory = _startDirectory;
	const RegexString& mask = _mask;
	const PathString& patternFileName = _patternFileName;
	const PathString& outputFileName = _outputFileName;
	
private:
	PathString _startDirectory;
	RegexString _mask;
	PathString _patternFileName;
	PathString _outputFileName;

	
	void parseArguments(const int argc, const PathChar** argv);
	void parsePairsOfArguments(const int argc, const PathChar** argv);
	void checkForEmpty() const;
	
};

#endif /* CRequest_h */

//
//  CRequest.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include <iostream>
#include "CRequest.hpp"

const std::string __exampleRequest = "MyApp.exe -p C:\\Temp -m *.* -i String.txt -o Result.txt";

CRequest::CRequest(const int argc, const char** argv) {
	parseArguments(argc, argv);
}

void CRequest::parseArguments(const int argc, const char **argv) {
		
	try {
		
		if(argc < 9) {
			throw "Too Few Arguments!";
		}
		
		parsePairsOfArguments(argc, argv);
		
		checkForEmpty();
		
	} catch (const std::string& s) {
		__log(s);
		throw ( "Error In Parsing Input Arguments\n" + s + "\nEXAMPLE:\n" + __exampleRequest );
	}
	
	__log("Parsing ended successfully");
	
}

void CRequest::parsePairsOfArguments(const int argc, const char **argv) {
	
	for(int iPairOfArg = 0; iPairOfArg < argc / 2; ++iPairOfArg) {
		const std::string key( argv[1 + iPairOfArg * 2] );
		const std::string value( argv[2 + iPairOfArg * 2] );
		const std::string runtimeDirectory(argv[0]);
		
		if( key == "-p" ) {
			_startDirectory = value;
			if(_startDirectory[1] != ':') {
				_startDirectory = runtimeDirectory + _startDirectory;
			}
		}
		else if( key == "-m" ) {
			_mask = value;
		}
		else if( key == "-i" ) {
			_patternFileName = value;
			if(_patternFileName[1] != ':') {
				_patternFileName = runtimeDirectory + _patternFileName;
			}
		}
		else if( key == "-o") {
			_outputFileName = value;
			if(_outputFileName[1] != ':') {
				_outputFileName = runtimeDirectory + _outputFileName;
			}
		}
		else if( key == "--nolog") {
			__log.turnOff();
			_nolog = true;
		}
		else {
			throw ( "Unknown Key : " + key );
		}
	}
}

void CRequest::checkForEmpty() const {
	
	if(_startDirectory.empty()) {
		throw "Not Found Start Directory Value";
	}
	
	if(_mask.empty()) {
		throw "Not Found Mask Value";
	}
	
	if(_patternFileName.empty()) {
		throw "Not Found Pattern File Name";
	}
	
	if(_outputFileName.empty()) {
		throw "Not Found Output File Name";
	}
}




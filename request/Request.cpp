//
//  CRequest.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include <iostream>
#include "Request.hpp"

const PathString __exampleRequest = getPathStringFromCString("MyApp.exe -p C:\\Temp -m *.* -i String.txt -o Result.txt");

Request::Request(const int argc, const PathChar** argv) {
	parseArguments(argc, argv);
}

void Request::parseArguments(const int argc, const PathChar **argv) {
		
	try {
		
		if(argc != 9) {
			for(int i = 0; i < argc; ++i) {
				cout << argv[i] << std::endl;
			}
			throw ( toPathString(argc) + getPathStringFromCString(" Is Too Few Arguments!") );
		}
		
		parsePairsOfArguments(argc, argv);
		
		checkForEmpty();
		
	} catch (const PathString& s) {
		throw (getPathStringFromCString("Error In Parsing Input Arguments\n") + s + getPathStringFromCString("\nEXAMPLE:\n") + __exampleRequest );
	}
	
	
}

void Request::parsePairsOfArguments(const int argc, const PathChar** argv) {
	
	for(int iPairOfArg = 0; iPairOfArg < argc / 2; ++iPairOfArg) {
		const PathString key( argv[1 + iPairOfArg * 2] );
		const PathString value( argv[2 + iPairOfArg * 2] );
		const PathString runtimeDirectory(argv[0]);

		
		if( key == getPathStringFromCString("-p") ) {

			_startDirectory = value;

			makeAbsolute(_startDirectory, runtimeDirectory);
		}
		else if( key == getPathStringFromCString("-m") ) {
			_mask = value;
		}
		else if( key == getPathStringFromCString("-i") ) {

			_patternFileName = value;
			
			makeAbsolute(_patternFileName, runtimeDirectory);
		}
		else if( key == getPathStringFromCString("-o")) {

			_outputFileName = value;
			
			makeAbsolute(_outputFileName, runtimeDirectory);
		}
		else {
			throw PathString(getPathStringFromCString("Unknown Key : ") + key );
		}
	}
}

void Request::checkForEmpty() const {
	
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

PathString Request::makeAbsolute(const PathString & path, const PathString& runtimeDIrectory) {

#if defined(__APPLE__)
	return runtimeDIrectory + getPathStringFromCString("/") + path;
#else
	return runtimeDIrectory + getPathStringFromCString("\\") + path;
#endif

}

bool Request::isAbsolute(const PathString & path) {

#if defined(__APPLE__)
	return (path[0] == '/');
#else
	return (path[1] == L':');
#endif

}




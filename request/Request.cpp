//
//  CRequest.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include <iostream>
#include "Request.hpp"

const PathString __exampleRequest = CHAR_TYPE"MyApp.exe -p C:\\Temp -m *.* -i String.txt -o Result.txt";

Request::Request(const int argc, const PathChar** argv) {
	parseArguments(argc, argv);
}

void Request::parseArguments(const int argc, const PathChar **argv) {
		
	try {
		
		if(argc != 9) {
			for(int i = 0; i < argc; ++i) {
				cout << argv[i] << std::endl;
			}
			throw std::string(toPathString(argc) + " Is Too Few Arguments!");
		}
		
		parsePairsOfArguments(argc, argv);
		
		checkForEmpty();
		
	} catch (const PathString& s) {
		throw ( CHAR_TYPE"Error In Parsing Input Arguments\n" + s + CHAR_TYPE"\nEXAMPLE:\n" + __exampleRequest );
	}
	
	
}

void Request::parsePairsOfArguments(const int argc, const PathChar** argv) {
	
	for(int iPairOfArg = 0; iPairOfArg < argc / 2; ++iPairOfArg) {
		const PathString key( argv[1 + iPairOfArg * 2] );
		const PathString value( argv[2 + iPairOfArg * 2] );
		const PathString runtimeDirectory(argv[0]);

		
		if( key == CHAR_TYPE"-p" ) {
			_startDirectory = value;
			if(_startDirectory[0] != '/') {
				_startDirectory = runtimeDirectory + "/" + _startDirectory;
			}
		}
		else if( key == CHAR_TYPE"-m" ) {
			_mask = value;
		}
		else if( key == CHAR_TYPE"-i" ) {
			_patternFileName = value;
			if(_patternFileName[0] != '/') {
				_patternFileName = runtimeDirectory + "/" + _patternFileName;
			}
		}
		else if( key == CHAR_TYPE"-o") {
			_outputFileName = value;
			if(_outputFileName[0] != '/') {
				_outputFileName = runtimeDirectory + "/" + _outputFileName;
			}
		}
		else {
			throw PathString( CHAR_TYPE"Unknown Key : " + key );
		}
	}
}

void Request::checkForEmpty() const {
	
	if(_startDirectory.empty()) {
		throw CHAR_TYPE"Not Found Start Directory Value";
	}
	
	if(_mask.empty()) {
		throw CHAR_TYPE"Not Found Mask Value";
	}
	
	if(_patternFileName.empty()) {
		throw CHAR_TYPE"Not Found Pattern File Name";
	}
	
	if(_outputFileName.empty()) {
		throw CHAR_TYPE"Not Found Output File Name";
	}
}




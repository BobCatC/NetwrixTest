//
//  Request.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <iostream>

#include "Request.hpp"
#include "../FileSystem/FileSystem.hpp"


/* ---------------------------------------- Request Static Data */

const std::string __exampleRequest = "MyApp.exe -p C:\\Temp -m *.* -i String.txt -o Result.txt";


/* ---------------------------------------- Request Constructor */

Request::Request() { }


/* ---------------------------------------- Request Parse Arguments Of Command Line */


void Request::parseArguments(const int argc, const char **argv)
{
		
	try {
		
		parsePairsOfArguments(argc, argv);
		
		checkForEmpty();
		
	} catch (const std::string& s) {
		throw ( "Error In Parsing Input Arguments\n" + s + "\nEXAMPLE:\n" + __exampleRequest );
	}
	
	
}


/* ---------------------------------------- Request Parse Pairse of Arguments ( key - value ) */

void Request::parsePairsOfArguments(const int argc, const char** argv)
{
	
	for(int iPairOfArg = 0; iPairOfArg < argc / 2; ++iPairOfArg) {

		const std::string key( argv[1 + iPairOfArg * 2] );
		const std::string value( argv[2 + iPairOfArg * 2] );
		
		
		if( key == "-p" ) {
			_startDirectory = value;
		}
		else if( key == "-m" ) {
			_mask = value;
		}
		else if( key == "-i" ) {
			_patternFileName = value;
		}
		else if( key == "-o" ) {
			_outputFileName = value;
		}
		else {
			throw ( "Unknown Key : " + key );
		}
	}
}


/* ---------------------------------------- Request Check If Any Field Is Empty */

void Request::checkForEmpty()
{
	
	if(_startDirectory.empty()) {
		throw "Not Found Start Directory Value";
	}
	
	if(_mask.empty()) {
		_mask = "*.*";
	}
	
	if(_patternFileName.empty()) {
		throw "Not Found Pattern File Name";
	}
	
	if(_outputFileName.empty()) {
		throw "Not Found Output File Name";
	}
}








//
//  Request.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <iostream>

#include "Request.hpp"
#include "FileSystem.hpp"


/* ---------------------------------------- Request Static Data */

const std::string __exampleRequest = "MyApp.exe -p C:\\Temp -m *.* -i String.txt -o Result.txt";


/* ---------------------------------------- Request Constructor */

Request::Request(const int argc, const char** argv)
{
	parseArguments(argc, argv);
}


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
		const std::string runtimeDirectory = bfs::current_path().string();
		
		
		if( key == "-p" ) {
			_startDirectory = value;

			makeAbsolute(_startDirectory, runtimeDirectory);
		}
		else if( key == "-m" ) {
			_mask = value;
		}
		else if( key == "-i" ) {
			_patternFileName = value;
			
			makeAbsolute(_patternFileName, runtimeDirectory);
		}
		else if( key == "-o" ) {
			_outputFileName = value;
			
			makeAbsolute(_outputFileName, runtimeDirectory);
		}
		else {
			throw ( "Unknown Key : " + key );
		}
	}
}


/* ---------------------------------------- Request Check If Any Field Is Empty */

void Request::checkForEmpty() const
{
	
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


/* ---------------------------------------- Request Make Path Absolute ( even if it's already abolute ) */

std::string Request::makeAbsolute(const std::string & path, const std::string& runtimeDIrectory)
{

	if (!isAbsolute(path)) {

#if defined(__APPLE__)
		return runtimeDIrectory + "/" + path;
#else
		return runtimeDIrectory + "\\" + path;
#endif
	}
	else {
		return path;
	}
}


/* ---------------------------------------- Request Check If Path Is Absolute */

bool Request::isAbsolute(const std::string & path)
{

#if defined(__APPLE__)
	return (path[0] == '/');
#else
	return (path[1] == L':');
#endif

}




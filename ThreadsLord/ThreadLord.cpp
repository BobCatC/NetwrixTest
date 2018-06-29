//
//  ThreadLord.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//
#include <iostream>

#include "ThreadLord.hpp"
#include "../Thread/ThreadSearcher.hpp"


/* ---------------------------------------- ThreadLord Constructor */

ThreadLord::ThreadLord(const Request& request, const unsigned int numberOfThreads):
	_request(request),
	_numberOfThreads(numberOfThreads),
	_tasks(numberOfThreads, request)
{
	openOutputFile();
	
	initRegexMask();
	
	startThreads();

	waitThreads();
	
	collectOutput();
}


/* ---------------------------------------- ThreadLord Destructor */

ThreadLord::~ThreadLord()
{
	if(_outputFile != nullptr) {
		fclose(_outputFile);
	}
	
	if(_buf != nullptr) {
		delete [] _buf;
	}
}


/* ---------------------------------------- ThreadLord Open Main Output File */

void ThreadLord::openOutputFile()
{
	_outputFile = fopen(_request.outputFileName.c_str(), "w");
	if(_outputFile == nullptr) {
		throw std::string( "Couldn't open output file \"" + _request.outputFileName + "\"" );
	}
}


/* ---------------------------------------- ThreadLord Init Regex From Mask String (different syntax) */

void ThreadLord::initRegexMask()
{
	const std::string& mask = _request.mask;
	std::string result;
	
	//  *  ->  .*
	//  .  ->  [.]
	//  ?  ->  .
	//  char -> char
	
	for(size_t i = 0; i < mask.size(); ++i) {
		switch (mask[i]) {
			case '*':
				result.append(".*");
				break;
			case '?':
				result.append(".");
				break;
			case '.':
				result.append("[.]");
				break;
			default:
				result.push_back(mask[i]);
				break;
		}
	}
	regexMask = std::regex(result);
}


/* ---------------------------------------- ThreadLord Start Threads */

void ThreadLord::startThreads()
{
	// we can't create either Zero or Too Many threads
	const unsigned int maxNumberOfThreads = 100;
	if(_numberOfThreads == 0 || _numberOfThreads > maxNumberOfThreads) {
		throw "Wrong Number Of Threads";
	}
	
	// all memory is Evenly distributed
	size_t cbMaxBufSize = _cbMaxBufSizeForProgramm / _numberOfThreads;
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		
		_threads.push_back(std::thread(threadSearcher, std::ref(_tasks), iThread, cbMaxBufSize, _request.patternFileName, std::ref(regexMask)));
		
	}
}

/* ---------------------------------------- ThreadLord Wait For All Threads To Compile Their Output Files Into Common */

void ThreadLord::waitThreads()
{
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		_threads[iThread].join();
	}
	
}


/* ---------------------------------------- ThreadLord  Compile Output Files Into Common */

void ThreadLord::collectOutput()
{
	_buf = new char[_cbMaxBufSizeForProgramm];
	
	for(size_t threadID = 0; threadID < _numberOfThreads; ++threadID) {
		try {
			getOutputOfThread(threadID);
		}
		catch(const std::string& err) {
			// nothing to do
		} catch(const char* err) {
			
		}
			
		}
	
	// _buf will be deleted in destructor
}


/* ---------------------------------------- ThreadLord  Compile Crt Thread File Into Common */

void ThreadLord::getOutputOfThread(size_t threadID)
{
	const std::string threadOutputFileName("output_" + std::to_string(threadID) + ".txt");
	
	FILE* threadOutputFile = fopen(threadOutputFileName.c_str(), "rb");
	if(threadOutputFile == nullptr) {
		return;
	}
	
	moveOutputOfThread(threadOutputFile);
	
	fclose(threadOutputFile);
//	remove(threadOutputFileName.c_str());
}


/* ---------------------------------------- ThreadLord Move Thread File Through Buf To Common */

void ThreadLord::moveOutputOfThread(FILE *threadOutputFile)
{
	size_t readSize = 0;
	
	// read -> check read size -> write
	while( 0 != ( readSize = fread(_buf, 1, _cbMaxBufSizeForProgramm, threadOutputFile) )) {
		fwrite(_buf, 1, readSize, _outputFile);
	}
}










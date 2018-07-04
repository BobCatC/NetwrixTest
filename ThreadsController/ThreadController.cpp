//
//  ThreadsController.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "ThreadController.hpp"
#include "../ThreadSearcher/ThreadSearcher.hpp"

/* ------------------------------------------------------------------- */
/* --------------------- class ThreadsController --------------------- */
/* ------------------------------------------------------------------- */


/* ---------------------------------------- ThreadsController Constructor */

ThreadsController::ThreadsController(const Request& request, const unsigned int numberOfThreads):
	_request(request),
	_numberOfThreads(numberOfThreads),
	_tasksBank(numberOfThreads, request)
{ }


/* ---------------------------------------- ThreadsController Destructor */

ThreadsController::~ThreadsController()
{
	if(_outputFile != nullptr) {
		fclose(_outputFile);
	}
	
	if(_buf != nullptr) {
		delete [] _buf;
	}
	
	for(const auto& threadOutputFileName : _threadsOutputFilesNames) {
		
		if(!threadOutputFileName.empty())
			remove(threadOutputFileName.c_str());
	}
}


void ThreadsController::createThreads()
{
	openOutputFile();
	
	initThreadsOutputFilesNames();
	
	initRegexMask();
	
	startThreads();
	
	waitThreads();
	
	mergeOutput();
}

/* ---------------------------------------- ThreadsController Open Main Output File */

void ThreadsController::openOutputFile()
{
	_outputFile = fopen(_request.outputFileName.c_str(), "w");
	if(_outputFile == NULL) {
		throw std::string( "Couldn't open output file \"" + _request.outputFileName + "\"" );
	}
}


void ThreadsController::initThreadsOutputFilesNames()
{
	_outputFileDirectory = bfs::path(_request.outputFileName).parent_path().string();
	
	if(_outputFileDirectory != "") {
		_outputFileDirectory += preferred_separator;
	}
	
	_threadsOutputFilesNames.resize(_numberOfThreads);
	
	for(unsigned int threadID = 0; threadID < _numberOfThreads; ++threadID) {
		
		const std::string pathWithoutExtension = _outputFileDirectory + "output_" + std::to_string(threadID);
		
		// to not to rewrite user's file
		_threadsOutputFilesNames[threadID] = findFreeName(pathWithoutExtension, "txt");
	}
	
}

/* ---------------------------------------- ThreadsController Init Regex From Mask String (different syntax) */

void ThreadsController::initRegexMask()
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


/* ---------------------------------------- ThreadsController Start Threads */

void ThreadsController::startThreads()
{
	// we can't create either Zero or Too Many threads
	const unsigned int maxNumberOfThreads = 100;
	if(_numberOfThreads == 0 || _numberOfThreads > maxNumberOfThreads) {
		throw "Wrong Number Of Threads";
	}
	
	// all memory is Evenly distributed
	size_t cbMaxBufSize = _cbMaxBufSizeForProgramm / _numberOfThreads;
	
	
	for(unsigned int iThread = 0; iThread < _numberOfThreads; ++iThread) {
		
		_threads.emplace_back(threadSearcher,
							  std::ref(_tasksBank),
							  iThread,
							  cbMaxBufSize,
							  std::ref(_outputFileDirectory),
							  std::ref(_threadsOutputFilesNames[iThread]),
							  _request.patternFileName,
							  std::ref(regexMask));
		
	}
}

/* ---------------------------------------- ThreadsController Wait For All Threads To Compile Their Output Files Into Common */

void ThreadsController::waitThreads()
{
	
	for(auto& thread : _threads) {
		thread.join();
	}
}


/* ---------------------------------------- ThreadsController  Compile Output Files Into Common */

void ThreadsController::mergeOutput()
{
	_buf = new char[_cbMaxBufSizeForProgramm];
	
	if(_buf == nullptr) {
		throw "Coludn't allocate mem";
	}
	
	for(unsigned int threadID = 0; threadID < _numberOfThreads; ++threadID) {
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


/* ---------------------------------------- ThreadsController  Compile Crt Thread File Into Common */

void ThreadsController::getOutputOfThread(size_t threadID)
{
	const std::string& threadOutputFileName = _threadsOutputFilesNames[threadID];
	
	FILE* threadOutputFile = fopen(threadOutputFileName.c_str(), "rb");
	if(threadOutputFile == NULL) {
		return;
	}
	
	moveOutputOfThread(threadOutputFile);
	
	fclose(threadOutputFile);
}


/* ---------------------------------------- ThreadsController Move Thread File Through Buf To Common */

void ThreadsController::moveOutputOfThread(FILE *threadOutputFile)
{
	size_t readSize = 0;
	
	// fread -> check read size -> write
	while( 0 != ( readSize = fread(_buf, 1, _cbMaxBufSizeForProgramm, threadOutputFile) )) {
		fwrite(_buf, 1, readSize, _outputFile);
	}
}










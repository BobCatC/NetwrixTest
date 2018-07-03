//
//  TaskExecutor.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <iostream>

#include "TaskExecutor.hpp"

void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from);


/* ---------------------------------------- TaskExecutor Constructor  */

TaskExecutor::TaskExecutor(const unsigned int threadID,
						   const std::string& thisThreadOutputFileName,
						   const size_t cbMaxBufSize,
						   const std::string& outputFileDirectory,
						   const std::string& patternFileName,
						   const std::regex& regexMask) :
_threadID(threadID),
_outputFileName(thisThreadOutputFileName),
_cbMaxBufSize(cbMaxBufSize),
_outputFileDirectory(outputFileDirectory),
_patternFileName(patternFileName),
_regexMask(regexMask)
{ }


/* ---------------------------------------- TaskExecutor init ----------------------------------------------------------- */
/* ---------------------------------------- "init" function was extracted from constructor because of possible exceptions */

void TaskExecutor::init()
{
	openDefaultFiles();
	
	initBuffers();
	
	countPiForFirstPatternFragment();
}

/* ---------------------------------------- TaskExecutor openDefaultFiles ----------------------------------------------------- */
/* ---------------------------------------- opens pattern and output (of this thread) files. Files will be closed in destructor */

void TaskExecutor::openDefaultFiles()
{
	_patternFile = fopen(_patternFileName.c_str(), "r");
	_thisThreadOutputFile = fopen(_outputFileName.c_str(), "w");
	
	if(_patternFile == NULL) {
		throw std::string( "Couldn't open pattern file" );
	}
	if(_thisThreadOutputFile == NULL) {
		throw std::string( "Couldn't open output file \"" + _outputFileName + "\"" );
	}
}


/* ---------------------------------------- TaskExecutor initBuffers  ------------------------------------- */
/* ---------------------------------------- allocates memory in "buf" and distributes it to "s" and "pi" */

void TaskExecutor::initBuffers()
{	
	_buffers.buf = new char[_cbMaxBufSize];
	
	if(_buffers.buf == nullptr) {
		throw "Memory Allocation Error";
	}
	
	// metrics of "pi" and "s"
	countDefaultMetrics();
	
	// metrics of pattern (len, fragment len, quontity of framgents)
	countPatternMetrics();
	
	_buffers.s = _buffers.buf;
	_buffers.pi = (int32_t*)(_buffers.buf + _buffers.sArraySize);
}


/* ---------------------------------------- TaskExecutor countDefaultMetrics ------------------- */
/* ---------------------------------------- length of "pi" and "s" is equal, but size is not   */

void TaskExecutor::countDefaultMetrics()
{
	_buffers.piArraySize = (_cbMaxBufSize * 4) / 5;
	_buffers.piArrayLen = _buffers.piArraySize / sizeof(*_buffers.pi);
	
	_buffers.sArraySize = (_cbMaxBufSize * 1) / 5;
	_buffers.sArrayLen = _buffers.sArraySize / sizeof(*_buffers.s);
}


/* ---------------------------------------- TaskExecutor countPatternMetrics ------------------------------------------------------- */
/* ---------------------------------------- gets the pattern file len, counts pattern fragment len and quontity of pattern fragments */

void TaskExecutor::countPatternMetrics()
{
	_patternMetrics.len = bfs::file_size(bfs::path(_patternFileName));
	_patternMetrics.fragmentLen = ((_buffers.sArrayLen - 1) * 1) / 7;
	
	// if pattern is little, we can set "_patternFragmentLen" to the size of whole pattern
	if(_patternMetrics.len < _patternMetrics.fragmentLen) {
		_patternMetrics.fragmentLen = _patternMetrics.len;
	}
	
	// round up
	_patternMetrics.numberOfFragments = (uint)( (_patternMetrics.len + _patternMetrics.fragmentLen - 1) / (_patternMetrics.fragmentLen) );
}


/* ---------------------------------------- TaskExecutor countTextMetrics ------------------------------ */
/* ---------------------------------------- does the same as countPatternMetrics ----------------------- */

void TaskExecutor::countTextMetrics()
{
	_textMetrics.len = bfs::file_size(_textFileBfsPath);
	_textMetrics.fragmentWithoutSuperimpositionLen = _buffers.sArraySize - 1 - _patternMetrics.fragmentLen;
	
	// every text fragment superimpose on previous one, "delta" of superimposition == "textFragmentWithoutSuperimpositionLen" - "patternFragmentLen"
	_textMetrics.fragmentWithSuperimpositionLen = _textMetrics.fragmentWithoutSuperimpositionLen - _patternMetrics.fragmentLen;
	
	// !!! it's important to count it in signed type
	_textMetrics.numberOfFragments = (uint)(   1 +
											( ((long)_textMetrics.len - (long)_patternMetrics.fragmentLen - 1 ) /
											 (long)(_textMetrics.fragmentWithoutSuperimpositionLen - _patternMetrics.fragmentLen) )   );
}


/* ---------------------------------------- TaskExecutor countPiForFirstPatternFragment --------------------------------------------------- */
/* ---------------------------------------- As We Always Count PrefixFunc For Combination Of First Pattern Fragment And Some Text Fragment, */
/* ---------------------------------------- We Can Save Result Of PrefixFunc For First Patter Fragment In File -----------------------------*/

void TaskExecutor::countPiForFirstPatternFragment()
{
	downloadFragment(_patternFile, 0, _patternMetrics.fragmentLen, _buffers.s);
	
	_buffers.pi[0] = 0;
	prefixFunction(_buffers.s, _buffers.pi, _patternMetrics.fragmentLen, 1);
	
	savePiForFirstPatternFragment();
}


/* ---------------------------------------- TaskExecutor savePiForFirstPatternFragment ---------------------------------------------------------------------- */
/* ---------------------------------------- Finds Free FileName In Directory Of OutputFile And Saves The Result Of Prefix Function For First Pattern Fragment */

void TaskExecutor::savePiForFirstPatternFragment()
{
	std::string piForFirstPatternFragmentFileNameWithoutExtension(_outputFileDirectory + "piForFirstFragment_" + std::to_string(_threadID));
	std::string extension("bin");
	
	_piForFirstPatternFragmentFileName = findFreeName(piForFirstPatternFragmentFileNameWithoutExtension, extension);
	
	_piForFirstPatternFragmentFile = fopen(_piForFirstPatternFragmentFileName.c_str(), "w");
	
	if(_piForFirstPatternFragmentFile == NULL) {
		throw std::string( "Couldn't open file \"" + _piForFirstPatternFragmentFileName );
	}
	
	uploadFragment(_piForFirstPatternFragmentFile, 0, _patternMetrics.fragmentLen, _buffers.pi);
	
	fclose(_piForFirstPatternFragmentFile);
	_piForFirstPatternFragmentFile = fopen(_piForFirstPatternFragmentFileName.c_str(), "rb");
	
	if(_piForFirstPatternFragmentFile == NULL) {
		throw std::string( "Couldn't open file \"" + _piForFirstPatternFragmentFileName );
	}
}


/* ---------------------------------------- TaskExecutor ~TaskExecutor ------------------------------ */
/* ---------------------------------------- Closes All Files, deleted own file, Frees Buf ----------- */

TaskExecutor::~TaskExecutor()
{
	if(_patternFile != NULL) {
		fclose(_patternFile);
	}
	if(_thisThreadOutputFile != NULL) {
		fclose(_thisThreadOutputFile);
	}
	if(_piForFirstPatternFragmentFile != NULL) {
		fclose(_piForFirstPatternFragmentFile);
		remove(_piForFirstPatternFragmentFileName.c_str());
	}
	
	if(_buffers.buf != nullptr) {
		delete [] _buffers.buf;
	}
}




/* ---------------------------------------- TaskExecutor doTask ------------------------------------------------------------ --*/
/* ---------------------------------------- Sets Name, Path, BfsPath Of TaskFile, Determines, If It's a File Or a Directrory --*/
/* ---------------------------------------- Here Is Main Try-Catch Block, As File Or Directory can be Unavailable ------------ */
/* ---------------------------------------- Triing To Process It Will Throw The Exception ------------------------------------ */
/* ---------------------------------------- We Don't Need To Process exception, As We Know, That The Reason Is Unaccessibility */

void TaskExecutor::doTask(const ThreadTask& task, std::vector<ThreadTask> &newTasksFiles, std::vector<ThreadTask> &newTasksDirectories)
{
	_textFileBfsPath = task.getFileBfsPath();
	
	_result.clear();
	
	// The main and the only Try-Catch block
	try {
		
		if(bfs::is_directory(_textFileBfsPath)) {
			processDirectory(newTasksFiles, newTasksDirectories);
		}
		else {
			_textFilePath = task.getFilePath();
			
			processFile();
		}
		
	} catch (const bfs::filesystem_error& err) {
		
	} catch(const std::string& err) {
		
	} catch(const char* err) {
		
	} catch(const std::exception& err) {
		
	}
	
	printResultToFile();
}


/* ---------------------------------------- TaskExecutor printResultToFile ------------------------------ */
/* ---------------------------------------- Firstly Prints Result Vector To The Buffer (faster) --------- */

void TaskExecutor::printResultToFile()
{
	if(!_result.empty()) {
		
		const size_t cbMaxResultBufSize = _cbMaxBufSize - 128;
		
		// If Initial Memory Size Is Too Little
		if(_cbMaxBufSize <= 128) {
			throw "Too little memory for thread";
		}
		
		size_t i = 0;
		size_t countOfEntries = _result.size();
		size_t cbResultBufSize = sprintf(_buffers.s, "%s\n", _textFilePath.c_str());
		
		while(i < countOfEntries) {
			
			while(i < countOfEntries && cbResultBufSize < cbMaxResultBufSize) {
				
				cbResultBufSize += sprintf(_buffers.s + cbResultBufSize, "\tposition : %i\n", _result[i]);
				++i;
			}
			
			fwrite(_buffers.s, 1, cbResultBufSize, _thisThreadOutputFile);
			cbResultBufSize = 0;
		}
		
	}
}













//
//  TaskExecutor.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <string.h>
#include <iostream>

#include "TaskExecutor.hpp"

void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from);



/*
 *
 *
 *
 
 
 
 */

/* ---------------------------------------- TaskExecutor Constructor  */

TaskExecutor::TaskExecutor(const unsigned int threadID,
						   const std::string& outputFileName,
						   const size_t cbMaxBufSize,
						   const std::string& outputFileDirectory,
						   const std::string& patternFileName,
						   const std::regex& regexMask) :
_threadID(threadID),
_outputFileName(outputFileName),
_cbMaxBufSize(cbMaxBufSize),
_outputFileDirectory(outputFileDirectory),
_patternFileName(patternFileName),
_regexMask(regexMask)
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
	_outputFile = fopen(_outputFileName.c_str(), "w");
	
	if(_patternFile == nullptr)
	{
		throw std::string( "Couldn't open pattern file" );
	}
	if(_outputFile == nullptr)
	{
		throw std::string( "Couldn't open output file \"" + _outputFileName + "\"" );
	}
}


/* ---------------------------------------- TaskExecutor initBuffers  ------------------------------------- */
/* ---------------------------------------- allocates memory in "_buf" and distributes it to "_s" and "_pi" */

void TaskExecutor::initBuffers()
{	
	_buf = new char[_cbMaxBufSize];
	
	if(_buf == nullptr) {
		throw "Memory Allocation Error";
	}
	
	// metrics of "_pi" and "_s"
	countDefaultMetrics();
	
	// metrics of pattern (len, fragment len, quontity of framgents)
	countPatternMetrics();
	
	_s = _buf;
	_pi = (int32_t*)(_buf + _sArraySize);
}


/* ---------------------------------------- TaskExecutor countDefaultMetrics ------------------- */
/* ---------------------------------------- length of "_pi" and "_s" is equal, but size is not   */

void TaskExecutor::countDefaultMetrics()
{
	_piArraySize = (_cbMaxBufSize * 4) / 5;
	_piArrayLen = _piArraySize / sizeof(int32_t);
	
	_sArraySize = (_cbMaxBufSize * 1) / 5;
	_sArrayLen = _sArraySize / sizeof(char);
}


/* ---------------------------------------- TaskExecutor countPatternMetrics ------------------------------------------------------- */
/* ---------------------------------------- gets the pattern file len, counts pattern fragment len and quontity of pattern fragments */

void TaskExecutor::countPatternMetrics()
{
	_patternLen = bfs::file_size(bfs::path(_patternFileName));
	_patternFragmentLen = ((_sArrayLen - 1) * 1) / 7;
	
	// if pattern is little, we can set "_patternFragmentLen" to the size of whole pattern
	if(_patternLen < _patternFragmentLen) {
		_patternFragmentLen = _patternLen;
	}
	
	// round up
	_numberOfFragmentsOfPattern = (_patternLen + _patternFragmentLen - 1) / (_patternFragmentLen);
}


/* ---------------------------------------- TaskExecutor countTextMetrics ------------------------------ */
/* ---------------------------------------- does the same as countPatternMetrics ----------------------- */

void TaskExecutor::countTextMetrics()
{
	_textLen = bfs::file_size(_bfsTextFilePath);
	_textFragmentLen = _sArraySize - 1 - _patternFragmentLen;
	
	// every text fragment superimpose on previous one, "delta" of superimposition == "_textFragmentLen" - "_patternFragmentLen"
	_textFragmentWithSuperimpositionLen = _textFragmentLen - _patternFragmentLen;
	
	// !!! it's important to count it in signed type
	_numberOfFragmentsOfTextWithSuperimposition = 1 + (((long long)_textLen - (long long)_patternFragmentLen - 1) / (long long)(_textFragmentLen - _patternFragmentLen));
}


/* ---------------------------------------- TaskExecutor countPiForFirstPatternFragment --------------------------------------------------- */
/* ---------------------------------------- As We Always Count PrefixFunc For Combination Of First Pattern Fragment And Some Text Fragment, */
/* ---------------------------------------- We Can Save Result Of PrefixFunc For First Patter Fragment In File -----------------------------*/

void TaskExecutor::countPiForFirstPatternFragment()
{
	downloadFragment(_patternFile, 0, _patternFragmentLen, _s);
	
	_pi[0] = 0;
	prefixFunction(_s, _pi, _patternFragmentLen, 1);
	
	savePiForFirstPatternFragment();
}


/* ---------------------------------------- TaskExecutor savePiForFirstPatternFragment ---------------------------------------------------------------------- */
/* ---------------------------------------- Finds Free FileName In Directory Of OutputFile And Saves The Result Of Prefix Function For First Pattern Fragment */

void TaskExecutor::savePiForFirstPatternFragment()
{
	std::string piForFirstPatternFragmentFileNameWithoutExtension(_outputFileDirectory + preferred_separator + "piForFirstFragment_" + std::to_string(_threadID));
	std::string extension("bin");
	
	_piForFirstPatternFragmentFileName = findFreeName(piForFirstPatternFragmentFileNameWithoutExtension, extension);
	
	_piForFirstPatternFragmentFile = fopen(_piForFirstPatternFragmentFileName.c_str(), "w");
	
	if(_piForFirstPatternFragmentFile == nullptr) {
		throw std::string( "Couldn't open file \"" + _piForFirstPatternFragmentFileName );
	}
	
	uploadFragment(_piForFirstPatternFragmentFile, 0, _patternFragmentLen * sizeof(*_pi), (char*)_pi);
	
	fclose(_piForFirstPatternFragmentFile);
	_piForFirstPatternFragmentFile = fopen(_piForFirstPatternFragmentFileName.c_str(), "rb");
}


/* ---------------------------------------- TaskExecutor ~TaskExecutor ------------------------------ */
/* ---------------------------------------- Closes All Files, Frees Buf ----------------------------- */

TaskExecutor::~TaskExecutor()
{
	if(_patternFile != nullptr) {
		fclose(_patternFile);
	}
	if(_outputFile != nullptr) {
		fclose(_outputFile);
	}
	if(_piForFirstPatternFragmentFile != nullptr) {
		fclose(_piForFirstPatternFragmentFile);
		remove(_piForFirstPatternFragmentFileName.c_str());
	}
	
	if(_buf != nullptr) {
		delete [] _buf;
	}
}






/* ---------------------------------------- TaskExecutor doTask ------------------------------------------------------------ */
/* ---------------------------------------- Sets Name, Path, BfsPath Of TaskFile, Determines, If It's a File Or a Directrory */
/* ---------------------------------------- Here Is Main Try-Catch Block, As File Or Directory Unavailable ----------------- */
/* ---------------------------------------- Triing To Process It Will Throw The Exception ---------------------------------- */
/* ---------------------------------------- We Don't Need To Process It, As We Know, That The Reason Is Unaccessibility ---- */

void TaskExecutor::doTask(const ThreadTask& task, std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories)
{
	_bfsTextFilePath = task.getFileBfsPath();
	_textFileNativeName = task.getFileNativeName();
	_textFilePath = task.getFilePath();
	
	_result.clear();
	
	try {
		
		if(bfs::is_directory(_bfsTextFilePath)) {
			processDirectory(newTasksFiles, newTasksDirectories);
		}
		else {
			processFile();
		}
		
	} catch (const bfs::filesystem_error& err) {
		
	} catch(const std::string& err) {
		
	} catch(const char* err) {
		
	}
	
	
	printResultToFile();
}


/* ---------------------------------------- TaskExecutor getRealPatternFragmentLen ------------------------------ */
/* ---------------------------------------- Last Fragment Is Less ----------------------------------------------- */

size_t TaskExecutor::getRealPatternFragmentLen(const size_t iPatternFragment)
{
	size_t realPatternFragmentLen;
	if(iPatternFragment == _numberOfFragmentsOfPattern - 1) {
		realPatternFragmentLen = _patternLen - (_numberOfFragmentsOfPattern - 1) * _patternFragmentLen;
	}
	else {
		realPatternFragmentLen = _patternFragmentLen;
	}
	return realPatternFragmentLen;
	
}


/* ---------------------------------------- TaskExecutor openDefaultFiles ------------------------------ */
/* ---------------------------------------- Last Fragment Is Less -------------------------------------- */

size_t TaskExecutor::getRealTextFragmentLen(const size_t iTextFragment)
{
	size_t realTextFragmentLen;
	if(iTextFragment == _numberOfFragmentsOfTextWithSuperimposition - 1) {
		realTextFragmentLen = _textLen - (_numberOfFragmentsOfTextWithSuperimposition - 1) * _textFragmentWithSuperimpositionLen;
	}
	else {
		realTextFragmentLen = _textFragmentLen;
	}
	return realTextFragmentLen;
}




/* ---------------------------------------- TaskExecutor fillResult ---------------------------- */
/* ---------------------------------------- Taskes All Remaining Entries And Fills Result Vector */

void TaskExecutor::fillResult(const std::vector<std::vector<EntryPair> >& entries)
{
	
	for(size_t iTextFragment = 0; iTextFragment < _numberOfFragmentsOfTextWithSuperimposition; ++iTextFragment)
	{
		for(const auto& pair : entries[iTextFragment])
		{
			const FirstFragmentEntry patternStartPosition = pair.first;
			_result.push_back((int)patternStartPosition);
		}

	}
}


/* ---------------------------------------- TaskExecutor printResultToFile ------------------------------ */
/* ---------------------------------------- Firstly Prints Result Vector To The Buffer (faster) --------- */

void TaskExecutor::printResultToFile()
{
	if(!_result.empty()) {
		
		const size_t cbMaxResultBufSize = _cbMaxBufSize - 128;
		
		// If Initial Memory Size Is Too Little
		assert(_cbMaxBufSize > 128);
		
		size_t i = 0;
		size_t size = _result.size();
		size_t cbResultBufSize = sprintf(_s, "*** In File %s\n", _textFilePath.c_str());
		
		while(i < size)
		{
			
			while(i < size && cbResultBufSize < cbMaxResultBufSize)
			{
				cbResultBufSize += sprintf(_s + cbResultBufSize, "\tposition : %i\n", _result[i]);
				++i;
			}
			
			fwrite(_s, 1, cbResultBufSize, _outputFile);
			cbResultBufSize = 0;
		}
		
		
	}
	
	
}













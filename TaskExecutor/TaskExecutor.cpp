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

namespace NetwrixTest {
	void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from);
}

/* ---------------------------------------- TaskExecutor Constructor  */

TaskExecutor::TaskExecutor(const unsigned int threadID,
						   BankOfTasks& tasksBank,
						   const std::string& thisThreadOutputFileName,
						   const size_t cbMaxBufSize,
						   const std::string& outputFileDirectory,
						   const std::string& patternFileName,
						   const std::regex& regexMask) :
_threadID(threadID),
_tasksBank(tasksBank),
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
	
	_buffers.initBuffers(_cbMaxBufSize, &_patternMetrics, _patternFileName);
	
	countPiForFirstPatternFragment();
}

/* ---------------------------------------- TaskExecutor openDefaultFiles ----------------------------------------------------- */
/* ---------------------------------------- opens pattern and output (of this thread) files. Files will be closed in destructor */

void TaskExecutor::openDefaultFiles()
{
	_patternFile = fopen_throw(_patternFileName.c_str(), "r");
	_thisThreadOutputFile = fopen_throw(_outputFileName.c_str(), "w");
}


/* ---------------------------------------- TaskExecutor countPiForFirstPatternFragment --------------------------------------------------- */
/* ---------------------------------------- As We Always Count PrefixFunc For Combination Of First Pattern Fragment And Some Text Fragment, */
/* ---------------------------------------- We Can Save Result Of PrefixFunc For First Patter Fragment In File -----------------------------*/

void TaskExecutor::countPiForFirstPatternFragment()
{
	downloadFragment(_patternFile, 0, _patternMetrics.fragmentLen, _buffers.s);
	
	_buffers.pi[0] = 0;
	NetwrixTest::prefixFunction(_buffers.s, _buffers.pi, _patternMetrics.fragmentLen, 1);
	
	savePiForFirstPatternFragment();
}


/* ---------------------------------------- TaskExecutor savePiForFirstPatternFragment ---------------------------------------------------------------------- */
/* ---------------------------------------- Finds Free FileName In Directory Of OutputFile And Saves The Result Of Prefix Function For First Pattern Fragment */

void TaskExecutor::savePiForFirstPatternFragment()
{
	std::string piForFirstPatternFragmentFileNameWithoutExtension(_outputFileDirectory + "piForFirstFragment_" + std::to_string(_threadID));
	std::string extension("bin");
	
	_piForFirstPatternFragmentFileName = findFreeName(piForFirstPatternFragmentFileNameWithoutExtension, extension);
	
	_piForFirstPatternFragmentFile = fopen_throw(_piForFirstPatternFragmentFileName.c_str(), "w");
	
	
	uploadFragment(_piForFirstPatternFragmentFile, 0, _patternMetrics.fragmentLen, _buffers.pi);
	
	fclose(_piForFirstPatternFragmentFile);
	_piForFirstPatternFragmentFile = fopen_throw(_piForFirstPatternFragmentFileName.c_str(), "rb");
	
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
	_textFilePath = task.getFilePath();

	_result.clear();
	
	// The main and the only Try-Catch block
	try {
		
		if(bfs::is_directory(_textFileBfsPath)) {
			processDirectory(newTasksFiles, newTasksDirectories);
		}
		else {
			
			processFile();
		}
		
	} catch (const bfs::filesystem_error& err) {
		printError();
	} catch(const std::string& err) {
		printError();
	} catch(const char* err) {
		printError();
	} catch(const std::exception& err) {
		printError();

	}
	
	printResultToFile();
}


/* ---------------------------------------- TaskExecutor printResultToFile ------------------------------ */

void TaskExecutor::printResultToFile()
{
	if(!_result.empty()) {
		
		fprintf(_thisThreadOutputFile, "%s\n", _textFilePath.c_str());
		
	}
}


/* ---------------------------------------- TaskExecutor printError ------------------------------ */
/* ---------------------------------------- In Try-Catch Block Checks The Reason Of The Error ---- */

void TaskExecutor::printError()
{
	try {
		if(bfs::exists(_textFileBfsPath)) {
			_tasksBank.log(_textFilePath + ": Permission denied");
		}
		else {
			_tasksBank.log(_textFilePath + ": No such file or directory");
		}
	} catch (const bfs::filesystem_error& err) {
		_tasksBank.log(_textFilePath + ": Filesystem access error");
	}
	
}













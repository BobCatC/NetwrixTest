//
//  TaskExecutor.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef TaskExecutor_hpp
#define TaskExecutor_hpp

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <stdio.h>

#include "../FileSystem/FileSystem.hpp"
#include "../ThreadTask/ThreadTask.hpp"
#include "FirstStepSearchInFile/FirstStepSearchInFile.hpp"
#include "SecondStepSearchInFile/SecondStepSearchInFile.hpp"
#include "CommonTypes/CommonTypes.hpp"

class TaskExecutor {

public:
	
	TaskExecutor(const unsigned int threadID,
				 const std::string& thisThreadOutputFileName,
				 const size_t cbMaxBufSize,
				 const std::string& outputFileDirectory,
				 const std::string& patternFileName,
				 const std::regex& regexMask);
	
	TaskExecutor(const TaskExecutor& exe) = delete;
	TaskExecutor(TaskExecutor&& exe) = delete;
	
	void init();
	
	~TaskExecutor();
	
	
	void doTask(const ThreadTask& task,
				std::vector<ThreadTask>& newTasksFiles,
				std::vector<ThreadTask>& newTasksDirectories);
	
	
private:
	/*---- Initial thread data */
	const unsigned int _threadID;
	const std::string& _outputFileName;
	const size_t _cbMaxBufSize;
	const std::string& _outputFileDirectory;
	const std::string& _patternFileName;
	const std::regex& _regexMask;

	/*---- Pointers at files --------------------------------------------------------------------------------------------------- */
	/*---- "_patternFile", "_thisThreadOutputFile" and "_piForFirstPatternFragmentFile" are always opened (closed in destructor) */
	/*---- "_textFile" is opened only while file processing -------------------------------------------------------------------- */
	FILE* _patternFile = nullptr;
	FILE* _piForFirstPatternFragmentFile = nullptr;
	FILE* _textFile = nullptr;
	FILE* _thisThreadOutputFile = nullptr;
	
	/*---- As we always count Prefix Function for first pattern fragment and different text fragment, */
	/*---- we can save the value of "_pi" for first pattern fragment in file and just updoal it ----- */
	std::string _piForFirstPatternFragmentFileName;
	
	bfs::path _textFileBfsPath;
	std::string _textFilePath;
	
	BuffersForSearch _buffers;
	
	/*---- Both the pattern and the text can't be loaded to memory (full file) ------ */
	/*---- We have to count the size of fragments (it depends on size of "s" buffer) */
	/*---- And count how many fragments we'll have ---------------------------------- */
	PatternMetrics _patternMetrics;
	
	/*---- The same with the text --------------------------------------------------------------------- */
	/*---- But to find the pattern correctly in the text we have to impose text fragments on each other */
	/*---- (pattern fragment entry can be at the junction of the text fragments, so we have to impose)  */
	TextMetrics _textMetrics;
	
	std::vector<PatternStartPosition> _result;
	
	
	
	void openDefaultFiles();
	void initBuffers();
	void countPiForFirstPatternFragment();
	void savePiForFirstPatternFragment();
	
	void countDefaultMetrics();
	void countPatternMetrics();
	void countTextMetrics();
	
	/*================== Directory Searching */

	void processDirectory(std::vector<ThreadTask> &newTasksFiles,
						  std::vector<ThreadTask> &newTasksDirectories);
	
	void destributeNewPath(const bfs::path& newPath,
						   std::vector<ThreadTask> &newTasksFiles,
						   std::vector<ThreadTask> &newTasksDirectories);
	
	
	/*================== File Searching */
	void processFile();
	
	void fillResult(const std::vector<std::vector<EntryPair>>& entries);
	void printResultToFile();
};


#endif /* TaskExecutor_hpp */

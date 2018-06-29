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

extern size_t numberOfFiles;
extern size_t fileDoneSize;

typedef unsigned int FirstFragmentEntry;
typedef unsigned int CrtFragmentEntry;
typedef std::pair<FirstFragmentEntry, CrtFragmentEntry> EntryPair;

class TaskExecutor {

public:
	
	TaskExecutor(const unsigned int threadID, const std::string& outputFileName, const size_t cbMaxBufSize, const std::string& patternFileName, const std::regex& regexMask);
	TaskExecutor(const TaskExecutor& exe) = delete;
	TaskExecutor(TaskExecutor& exe) = delete;

	~TaskExecutor();
	
	
	void doTask(const ThreadTask& task, std::vector<std::string>& newTasksFiles, std::vector<std::string>& newTasksDirectories);
	size_t doneSize = 0;
	size_t doneFiles = 0;
	size_t doneDirectories = 0;
	int allTimePrefix = 0;
	
private:
	
	const unsigned int _threadID;
	const std::string& _outputFileName;
	const std::regex& _regexMask;
	const size_t _cbMaxBufSize;
	const std::string& _patternFileName;
	
	
	FILE* _patternFile = nullptr;
	FILE* _textFile = nullptr;
	FILE* _outputFile = nullptr;
	
	bfs::path _path;
	std::string _textFileNativeName;
	std::string _textFilePath;
	
	char* _buf;
	char* _s;
	int32_t* _pi = nullptr;

	
	size_t _piArraySize, _piArrayLen;
	size_t _sArraySize, _sArrayLen;
	
	size_t _patternLen;
	size_t _patternFragmentLen;
	size_t _numberOfFragmentsOfPattern;
	
	size_t _textLen;
	size_t _textFragmentLen;
	size_t _textFragmentWithImpositionLen;
	size_t _numberOfFragmentsOfTextWithImposition;
	
	std::vector<FirstFragmentEntry> _result;
	
	
	void openDefaultFiles();
	void initBuffers();
	
	void countDefaultMetrics();
	void countPatternMetrics();
	void countTextMetrics();
	
	void processDirectory(std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories);
	void destributeNewPath(const bfs::path& newPath, std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories);
	
	void processFile();
	void searchWithPrefixFunc(const size_t realPatternFragmentLen, const size_t iTextFragment, const size_t len, std::vector<EntryPair>& result);
	void findEntriesOfFirstFragment(std::vector<std::vector<EntryPair>>& entries);
	bool firstPatternFragmentCanBeOnPosition(size_t position, size_t iTextFragment);
	bool patternFragmentFitsTextFragment(const char* patternFragment, size_t realPatternFragmentLen, const char* textFragment, size_t realTextFragmentLen, size_t positionInTextFragment);
	
	
	void siftEntries(std::vector<std::vector<EntryPair>>& entries);
	void increasePositions(std::vector<std::vector<EntryPair>>& entries);
	
	
	void fillResult(const std::vector<std::vector<EntryPair>>& entries);
	
	size_t getRealPatternFragmentLen(const size_t iPatternFragment);
	size_t getRealTextFragmentLen(const size_t iTextFragment);
	
	void printResultToFile();
};


#endif /* TaskExecutor_hpp */

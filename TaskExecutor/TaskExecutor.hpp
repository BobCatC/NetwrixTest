//
//  TaskExecutor.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef TaskExecutor_hpp
#define TaskExecutor_hpp

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <stdio.h>

#include "../PathString.hpp"
#include "../FileSystem/FileSystem.hpp"
#include "../ThreadTask/ThreadTask.hpp"

extern size_t numberOfFiles;
extern size_t fileDoneSize;

typedef unsigned int FirstFragmentEntry;
typedef unsigned int CrtFragmentEntry;
typedef std::pair<FirstFragmentEntry, CrtFragmentEntry> EntryPair;

class TaskExecutor {

public:
	
	TaskExecutor(const unsigned int threadID, const size_t cbMaxBufSize, const std::string& patternFileName, const std::regex& regexMask);
	TaskExecutor(const TaskExecutor& exe) = delete;
	TaskExecutor(TaskExecutor& exe) = delete;

	~TaskExecutor();
	
	
	const std::vector<FirstFragmentEntry>& doTask(const ThreadTask& task, std::vector<std::string>& newTasksFiles, std::vector<std::string>& newTasksDirectories);
	size_t doneSize = 0;
	size_t doneFiles = 0;
	int allTimePrefix = 0;
private:
	
	const unsigned int _threadID;
	const std::regex& _regexMask;
	const size_t _cbMaxBufSize;
	const std::string& _patternFileName;
	
	
	FILE* _patternFile = nullptr;
	FILE* _textFile = nullptr;
	
	char* _buf;
	char* _s;
	int32_t* _pi = nullptr;

	
	size_t _piArraySize, _piArrayLen;
	size_t _sArraySize, _sArrayLen;
	
	size_t _patternLen;
	size_t _patternFragmentLen;
	size_t _numberOfFragmentsOfPattern;
	
	
	bfs::path _path;
	std::string _textFileNativeName;
	std::string _textFilePath;
	
	size_t _textLen;
	size_t _textFragmentLen;
	size_t _textFragmentWithImpositionLen;
	size_t _numberOfFragmentsOfTextWithImposition;
	
	std::vector<FirstFragmentEntry> _result;

	
	
	void countDefaultMetrics();
	void countPatternMetrics();
	void countTextMetrics();
	
	void countFirstFragmentOfPattern();
	
	void processDirectory(std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories);
	
	void processFile();
	void searchInFile(const size_t cbMaxBufSize, char* buf, const std::string& patternFileName);
	
	
	
	void findEntriesOfFirstFragment(std::vector<std::vector<EntryPair>>& entries);
	
	void siftEntries(std::vector<std::vector<EntryPair>>& entries);
	
	bool firstPatternFragmentCanBeOnPosition(size_t position, size_t iTextFragment);
	bool patternFragmentFitsTextFragment(const char* patternFragment, size_t realPatternFragmentLen, const char* textFragment, size_t realTextFragmentLen, size_t positionInTextFragment);
	void increasePositions(std::vector<std::vector<EntryPair>>& entries);
	void fillResult(const std::vector<std::vector<EntryPair>>& entries);
	
	void searchWithPrefixFunc(const size_t realPatternFragmentLen, const size_t iTextFragment, const size_t len, std::vector<EntryPair>& result);
	
	size_t getRealPatternFragmentLen(const size_t iPatternFragment);
	size_t getRealTextFragmentLen(const size_t iTextFragment);
};


#endif /* TaskExecutor_hpp */

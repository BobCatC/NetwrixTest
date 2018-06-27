//
//  ThreadTask.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CThreadTask_hpp
#define CThreadTask_hpp

#define _CRT_SECURE_NO_WARNINGS

#include <boost/filesystem.hpp>
#include <regex>
#include <set>
#include <stdio.h>
#include <iostream>
namespace bfs = boost::filesystem;

#include "../PathString.hpp"


class ThreadTask {
	
public:
	
	ThreadTask(const bfs::path& path = bfs::path());
	ThreadTask(const ThreadTask& other);
//	ThreadTask(ThreadTask& other) = delete;
	
	~ThreadTask();
	
	void doTask(const size_t cbMaxBufSize, char* buf, std::vector<ThreadTask>& newTasksFiles, std::vector<ThreadTask>& newTasksDirectories, const PathString& patternFileName, const regex& regexMask, std::vector<std::string>& result, std::vector<std::vector<std::set<int32_t>>>& entries);
	
	const PathString& getFileName() const { return _path.native(); }
	
	const PathString& getFilePath() const {
		
#ifdef __APPLE__
		return _path.string(); 
#else
		return _path.wstring();
#endif
	}
	
private:
	const bfs::path _path;
	
	size_t patternFragmentLen;
	size_t textFragmentLen;
	size_t numberOfFragmentsOfPattern;
	size_t numberOfFragmentsOfTextWithImposition;
	
	std::vector<std::string>* _result;
	std::vector<std::vector<std::set<int32_t>>>* _entries;
	
	void processDirectory(std::vector<ThreadTask>& newTasksFiles, std::vector<ThreadTask> &newTasksDirectories, const regex& regexMask);
	
	void processFile(const size_t cbMaxBufSize, char* buf, const PathString& patternFileName);
	void searchInFile(const size_t cbMaxBufSize, char* buf, const PathString& patternFileName);
	void downloadFragment(FILE* file, size_t fromPosition, size_t size, char* buf) const;
	void find(size_t crtPatternFragment, size_t iTextFragmentInWhichToFind, size_t iPositionWhereToFind, size_t startPatternPosition);
	
};






#endif /* CThreadTask_hpp */

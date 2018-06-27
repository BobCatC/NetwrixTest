//
//  ThreadTask.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CThreadTask_hpp
#define CThreadTask_hpp

#include <boost/filesystem.hpp>
#include <regex>
#include <set>
namespace bfs = boost::filesystem;
#include "MyVector.hpp"



class ThreadTask {
	
public:
	
	ThreadTask(const bfs::path& path = bfs::path());
	ThreadTask(const ThreadTask& other);
//	ThreadTask(ThreadTask& other) = delete;
	
	~ThreadTask();
	
	void doTask(const size_t cbMaxBufSize, char* buf, MyVector<ThreadTask>& newTasksFiles, MyVector<ThreadTask>& newTasksDirectories, const std::string& patternFileName, const std::regex& regexMask, MyVector<std::string>& result, MyVector<std::vector<std::set<int32_t>>>& entries);
	
	const bfs::path::string_type& getFileName() const { return _path.native(); }
	
	const bfs::path::string_type& getFilePath() const {
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
	
	MyVector<std::string>* _result;
	MyVector<std::vector<std::set<int32_t>>>* _entries;
	
	void processDirectory(MyVector<ThreadTask>& newTasksFiles, MyVector<ThreadTask> &newTasksDirectories, const std::regex& regexMask);
	
	void processFile(const size_t cbMaxBufSize, char* buf, const std::string& patternFileName);
	void searchInFile(const size_t cbMaxBufSize, char* buf, const std::string& patternFileName);
	void downloadFragment(FILE* file, size_t fromPosition, size_t size, char* buf) const;
	void find(size_t crtPatternFragment, size_t iTextFragmentInWhichToFind, size_t iPositionWhereToFind, size_t startPatternPosition);
	
};






#endif /* CThreadTask_hpp */

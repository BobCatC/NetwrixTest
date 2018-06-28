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

#include <regex>
#include <set>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "../PathString.hpp"
#include "../FileSystem/FileSystem.hpp"

class ThreadTask {
	
public:
	
	ThreadTask(const bfs::path& path = bfs::path());
	ThreadTask(const ThreadTask& other);
	
	~ThreadTask();
	
	const std::string getFileName() const;
	
	const std::string& getFilePath() const;
	
	const bfs::path& getPath() const;
	
private:
	const bfs::path _path;
	
	size_t patternFragmentLen;
	size_t textFragmentLen;
	size_t numberOfFragmentsOfPattern;
	size_t numberOfFragmentsOfTextWithImposition;
	
};






#endif /* CThreadTask_hpp */

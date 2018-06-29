//
//  TaskExecutorDirectoryProcessing.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 29.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include "TaskExecutor.hpp"


/* ======================================== TaskExecutor Directory Processing Functions ======================================== */

/* ---------------------------------------- TaskExecutor processDirectory ------------------------------ */

void TaskExecutor::processDirectory(std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories)
{
	const auto endIt = bfs::directory_iterator();
	for(auto it = bfs::directory_iterator(_bfsTextFilePath); it != endIt; ++it) {
		
		const bfs::path& newFilePath = it->path();
		
		destributeNewPath(newFilePath, newTasksFiles, newTasksDirectories);
		
	}
}


/* ---------------------------------------- TaskExecutor destributeNewPath ------------------------------ */
/* ---------------------------------------- Already Here Files Are Separated With Mask ------------------ */

void TaskExecutor::destributeNewPath(const bfs::path &newFilePath, std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories)
{
	
	if(bfs::is_directory(newFilePath)) {
		newTasksDirectories.push_back(newFilePath.string());
	}
	else {
		const std::string nativeName = newFilePath.filename().string();
		
		if (fileFitsMask(nativeName, _regexMask)) {
			newTasksFiles.push_back(newFilePath.string());
		}
	}
	
}

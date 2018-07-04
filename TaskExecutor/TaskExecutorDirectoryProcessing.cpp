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

void TaskExecutor::processDirectory(std::vector<ThreadTask> &newTasksFiles, std::vector<ThreadTask> &newTasksDirectories)
{
	const auto endIt = bfs::directory_iterator();
	for(auto it = bfs::directory_iterator(_textFileBfsPath); it != endIt; ++it) {
		
		const bfs::path& newFilePath = it->path();
		
		destributeNewPath(newFilePath, newTasksFiles, newTasksDirectories);
		
	}
}


/* ---------------------------------------- TaskExecutor destributeNewPath ------------------------------ */
/* ---------------------------------------- Already Here Files Are Separated With Mask ------------------ */

void TaskExecutor::destributeNewPath(const bfs::path &newFilePath, std::vector<ThreadTask> &newTasksFiles, std::vector<ThreadTask> &newTasksDirectories)
{
	
	if(bfs::is_directory(newFilePath)) {
		newTasksDirectories.emplace_back(newFilePath);
	}
	else {
		const std::string nativeName = newFilePath.filename().string();
		
		if (fileFitsMask(nativeName, _regexMask)) {
			newTasksFiles.emplace_back(newFilePath);
		}
	}
	
}

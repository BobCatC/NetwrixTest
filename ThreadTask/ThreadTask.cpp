//
//  ThreadTask.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include "ThreadTask.hpp"


/* ---------------------------------------- ThreadTask Constructor  */

ThreadTask::ThreadTask(const bfs::path& path) : _bfsPath(path){ }


/* ---------------------------------------- ThreadTask Copy Constructor */

ThreadTask::ThreadTask(const ThreadTask& other) : _bfsPath(other._bfsPath) { }


/* ---------------------------------------- ThreadTask getFileName  */

const std::string ThreadTask::getFileNativeName() const
{
	return _bfsPath.filename().string();
}


/* ---------------------------------------- ThreadTask getFilePath  */

const std::string ThreadTask::getFilePath() const
{
	return _bfsPath.string();
}


/* ---------------------------------------- ThreadTask getFileBfsPath  */

const bfs::path& ThreadTask::getFileBfsPath() const
{
	return _bfsPath;
}















//
//  ThreadTask.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include "ThreadTask.hpp"


/* ---------------------------------------- ThreadTask Constructor  */

ThreadTask::ThreadTask(const bfs::path& path) : _path(path){ }


/* ---------------------------------------- ThreadTask Copy Constructor */

ThreadTask::ThreadTask(const ThreadTask& other) : _path(other._path) { }


/* ---------------------------------------- ThreadTask getFileName  */

const std::string ThreadTask::getFileName() const
{
	return _path.filename().string();
}


/* ---------------------------------------- ThreadTask getFilePath  */

const std::string ThreadTask::getFilePath() const
{
	return _path.string();
}


/* ---------------------------------------- ThreadTask getFileBfsPath  */

const bfs::path& ThreadTask::getFileBfsPath() const
{
	return _path;
}















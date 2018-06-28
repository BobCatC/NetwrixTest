//
//  ThreadTask.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "ThreadTask.hpp"


ThreadTask::ThreadTask(const bfs::path& path) : _path(path){ }

ThreadTask::ThreadTask(const ThreadTask& other) : _path(other._path) { }

ThreadTask::~ThreadTask() { }

const std::string ThreadTask::getFileName() const
{
	return _path.filename().string();
}

const std::string& ThreadTask::getFilePath() const
{
	return _path.string();
}

const bfs::path& ThreadTask::getPath() const
{
	return _path;
}















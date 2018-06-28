//
//  FileSystem.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef FileSystem_h
#define FileSystem_h

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <regex>

#if defined(_WIN32) || defined(_WIN64)

#	include <experimental/filesystem>
	namespace bfs = std::experimental::filesystem;

#else

#	include <boost/filesystem.hpp>
	namespace bfs = boost::filesystem;

#endif

enum BoolExtension {
	True, False, Undeterminacy
};


bool canBeOpened(const std::string& absolutePath);

BoolExtension isRegular(const bfs::path& path);

bool fileFitsMask(const std::string& fileNativeName, const std::regex& regexMask);

void downloadFragment(FILE *file, size_t fromPosition, size_t size, char *buf);
void uploadFragment(FILE* file, size_t fromPosition, size_t size, char* buf);

#endif /* FileSystem_h */

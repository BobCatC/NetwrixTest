//
//  FileSystem.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef FileSystem_h
#define FileSystem_h

/* ---------------------------------------- FileSystem "ifdef" including of modules  */

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <regex>


#if defined(_WIN32) || defined(_WIN64)
/* ---------------------------------------- WindowsNT */
#	include <experimental/filesystem>
	namespace bfs = std::experimental::filesystem;

#else

#	if (defined (__APPLE__) && defined(__MACH__)) || defined(__unix__) || defined(__linux__)
/* ---------------------------------------- Unux/unix-like */
#		include <boost/filesystem.hpp>
		namespace bfs = boost::filesystem;

#	else
#		error "Unknown Environment"
#	endif

#endif



bool fileWithNameExists(const std::string& path);


bool fileFitsMask(const std::string& fileNativeName, const std::regex& regexMask);

void downloadFragment(FILE *file, size_t fromPositionInFile, size_t size, char *buf);
void uploadFragment(FILE* file, size_t fromPositionInFile, size_t size, char* buf);

#endif /* FileSystem_h */

//
//  FileSystem.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef FileSystem_h
#define FileSystem_h

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <regex>

/* ---------------------------------------- FileSystem "ifdef" including of modules  */

#if defined(_WIN32) || defined(_WIN64)
/* ---------------------------------------- Windows */
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



extern const std::string preferred_separator;


bool fileWithNameExists(const std::string& path);
std::string findFreeName(const std::string& pathWithoutExtension, const std::string& extension);


bool fileFitsMask(const std::string& fileNativeName, const std::regex& regexMask);


/* ---------------------------------------- FileSystem downloadFragment  */

template<class value_type>
auto downloadFragment(FILE *file, size_t fromPositionInFile, size_t len, value_type *buf)
{
	fseek(file, fromPositionInFile, SEEK_SET);
	return fread(buf, sizeof(value_type), len, file);
}


/* ---------------------------------------- FileSystem uploadFragment */

template<class value_type>
auto uploadFragment(FILE* file, size_t fromPositionInFile, size_t len, value_type *buf)
{
	fseek(file, fromPositionInFile, SEEK_SET);
	return fwrite(buf, sizeof(value_type), len, file);
}

#endif /* FileSystem_h */





















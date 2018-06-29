//
//  FileSystem.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "FileSystem.hpp"


#if defined(_WIN32) || defined(_WIN64)
/* ---------------------------------------- WindowsNT */
	const std::string preferred_separator("\\");
#else

#	if (defined (__APPLE__) && defined(__MACH__)) || defined(__unix__) || defined(__linux__)
/* ---------------------------------------- Unux/unix-like */
		const std::string preferred_separator("/");
#	else
#		error "Unknown Environment"
#	endif

#endif



/* ---------------------------------------- FileSystem   */

void downloadFragment(FILE *file, size_t fromPositionInFile, size_t size, char *buf)
{
	fseek(file, fromPositionInFile, SEEK_SET);
	fread(buf, 1, size, file);
}


/* ---------------------------------------- FileSystem   */

void uploadFragment(FILE* file, size_t fromPositionInFile, size_t size, char* buf)
{
	fseek(file, fromPositionInFile, SEEK_SET);
	fwrite(buf, 1, size, file);
}


/* ---------------------------------------- FileSystem   */

bool fileFitsMask(const std::string& nativeName, const std::regex& regexMask)
{
	return std::regex_match(nativeName, regexMask);
}


/* ---------------------------------------- FileSystem   */

bool fileWithNameExists(const std::string& path)
{
	bool res;
	try {
		res = bfs::exists(bfs::path(path));
	} catch (const bfs::filesystem_error& err) {
		res = true;
	}
	
	return res;
}


std::string findFreeName(const std::string& pathWithoutExtension, const std::string& extension)
{
	std::string resultWithoutExtension(pathWithoutExtension);
	
	while(fileWithNameExists(resultWithoutExtension + '.' + extension)) {
		
		char randomEnglishChar = 'a' + (rand() % ('z' - 'a' + 1));
		resultWithoutExtension.push_back(randomEnglishChar);
	}
	return (resultWithoutExtension + '.' + extension);
}










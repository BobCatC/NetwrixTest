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

// bfs::path::preferred_separator hasn't method to convert to std::string
const std::string preferred_separator = bfs::path(bfs::path::string_type(&bfs::path::preferred_separator)).string();


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
	return (resultWithoutExtension + "." + extension);
}


FILE* fopen_throw(const char* fileName, const char* mode)
{
	FILE* file = fopen(fileName, mode);
	if(file == NULL) {
		throw std::string("Couldn't open \"" + std::string(fileName) + "\"");
	}
	
	return file;
}














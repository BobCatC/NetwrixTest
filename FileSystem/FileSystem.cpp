//
//  FileSystem.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "FileSystem.hpp"


void downloadFragment(FILE *file, size_t fromPosition, size_t size, char *buf)
{
	fseek(file, fromPosition, SEEK_SET);
	fread(buf, 1, size, file);
}

void uploadFragment(FILE* file, size_t fromPosition, size_t size, char* buf)
{
	fseek(file, fromPosition, SEEK_SET);
	fwrite(buf, 1, size, file);
}

bool fileFitsMask(const std::string& nativeName, const std::regex& regexMask)
{
	return std::regex_match(nativeName, regexMask);
}

bool canBeOpened(const std::string& absolutePath) {
	
#if defined(_WIN32) || defined(_WIN64)
	
	bool res = true;

	try
	{
		const bfs::path path(absolutePath);
		if (bfs::is_directory(path)) {
			bfs::directory_iterator(path);
		}
		else {
			FILE* file = fopen(absolutePath.c_str(), "r");
			res = (file != nullptr);
			if(file != nullptr)
				fclose(file);
		}
	}
	catch (const bfs::filesystem_error& e)
	{
		return false;
	}

	return res;
#else
	FILE* file = fopen( absolutePath.c_str() , "r");
	bool res = (file != NULL);
	if(file != NULL) {
		fclose(file);
	}
	return res;
#endif
}

BoolExtension isRegular(const bfs::path& path)
{
	try {
		if(bfs::is_directory(path)) {
			return False;
		}
		else {
			return True;
		}
	} catch (const bfs::filesystem_error& e) {
		return Undeterminacy;
	}
}

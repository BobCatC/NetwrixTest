//
//  FileSystem.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

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
	FILE* file;
	fopen_s(&file, absolutePath.c_str(), "r");
	bool res = (file != NULL);
	if (file != NULL) {
		fclose(file);
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
		if(bfs::is_regular_file(path)) {
			return True;
		}
		else {
			return False;
		}
	} catch (const bfs::filesystem_error& e) {
		return Undeterminacy;
	}
}

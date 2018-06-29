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


void downloadFragment(FILE *file, size_t fromPositionInFile, size_t size, char *buf)
{
	fseek(file, fromPositionInFile, SEEK_SET);
	fread(buf, 1, size, file);
}

void uploadFragment(FILE* file, size_t fromPositionInFile, size_t size, char* buf)
{
	fseek(file, fromPositionInFile, SEEK_SET);
	fwrite(buf, 1, size, file);
}

bool fileFitsMask(const std::string& nativeName, const std::regex& regexMask)
{
	return std::regex_match(nativeName, regexMask);
}


bool fileWithNameExists(const std::string& path)
{
	FILE* file = fopen(path.c_str(), "r");
	bool res = (file != nullptr);
	if(file != nullptr) {
		fclose(file);
	}
	
	return res;
}

//
//  MemoryKeeper.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 26.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include <stdio.h>

size_t FILE_OPEN_COUNTER = 0;
size_t MAX_OPENED = 0;

FILE* fopen_keep(const char* name, const char* mode) {
	FILE* file = fopen(name, mode);
	if(file != NULL)
		++FILE_OPEN_COUNTER;
	if(FILE_OPEN_COUNTER > MAX_OPENED) {
		MAX_OPENED = FILE_OPEN_COUNTER;
	}
	return file;
}

int fclose_keep(FILE* file) {
	int res = fclose(file);
	if(res == 0)
		--FILE_OPEN_COUNTER;
	
	return res;
}

//
//  MemoryKeeper.h
//  NetwrixTest
//
//  Created by Александр Пахомов on 26.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef MemoryKeeper_h
#define MemoryKeeper_h

extern size_t FILE_OPEN_COUNTER;
extern size_t MAX_OPENED;

FILE* fopen_keep(const char*, const char*);
int fclose_keep(FILE* file);

#endif /* MemoryKeeper_h */

//
//  CThreadSearcher.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef ThreadSearcher_hpp
#define ThreadSearcher_hpp


#include <vector>
#include <atomic>
#include <iostream>
#include <regex>
#include <fstream>

#include "../BankOfTasks/BankOfTasks.hpp"
#include "../PathString.hpp"
#include "../TaskExecutor/TaskExecutor.hpp"

void threadSearcher(BankOfTasks& tasks, const unsigned int threadID, const size_t cbBufSize, const std::string& patternFileName, const std::regex& regexMask);

#endif /* ThreadSearcher_hpp */

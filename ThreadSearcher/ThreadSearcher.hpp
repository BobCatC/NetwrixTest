//
//  CThreadSearcher.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef ThreadSearcher_hpp
#define ThreadSearcher_hpp



#include <regex>

#include "../BankOfTasks/BankOfTasks.hpp"

void threadSearcher(BankOfTasks& tasks,
					const unsigned int threadID,
					const size_t cbBufSize,
					const std::string& outputFileDirectory,
					const std::string& patternFileName,
					const std::regex& regexMask);

#endif /* ThreadSearcher_hpp */

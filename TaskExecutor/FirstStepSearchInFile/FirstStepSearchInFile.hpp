//
//  FirstStepSearchInFile.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 02.07.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef FirstStepSearchInFile_hpp
#define FirstStepSearchInFile_hpp

#include "../CommonTypes.hpp"

class FirstStepSearchInFile {
	
public:
	FirstStepSearchInFile(const BuffersForSearch& buffers,
						  const PatternMetrics& patternMetrics,
						  const TextMetrics& textMetrics,
						  FILE* patternFile,
						  FILE* piForFirstPatternFragmentFile,
						  FILE* textFile);
	
	void findEntriesOfFirstFragment(std::vector<std::vector<EntryPair>>& entries);
	
private:
	
	const BuffersForSearch& _buffers;
	const PatternMetrics& _patternMetrics;
	const TextMetrics& _textMetrics;
	FILE* _patternFile;
	FILE* _piForFirstPatternFragmentFile;
	FILE* _textFile;
	
	
	bool searchWithPrefixFunc(const size_t realPatternFragmentLen,
						 const uint iTextFragment,
						 const size_t len,
						 std::vector<EntryPair>& result);
	
	bool firstPatternFragmentCanBeOnPosition(const size_t position, const uint iTextFragment);
};




#endif /* FirstStepSearchInFile_hpp */

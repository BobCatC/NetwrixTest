//
//  SecondStepSearchInFile.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 02.07.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef SecondStepSearchInFile_hpp
#define SecondStepSearchInFile_hpp

#include <vector>

#include "../CommonTypes.hpp"
#include "../../FileSystem/FileSystem.hpp"

class SecondStepSearchInFile {

public:
	SecondStepSearchInFile(const BuffersForSearch& buffers,
						   const PatternMetrics& patternMetrics,
						   const TextMetrics& textMetrics,
						   FILE* patternFile,
						   FILE* textFile);
	
	void siftEntries(std::vector<std::vector<EntryPair>>& entries);
	
	
private:
	
	const BuffersForSearch& _buffers;
	const PatternMetrics& _patternMetrics;
	const TextMetrics& _textMetrics;
	FILE* _patternFile;
	FILE* _textFile;
	
	char* _crtPatternFragment = nullptr;
	size_t _realPatternFragmentLen = 0;
	
	char* _crtTextFragment = nullptr;
	size_t _realCrtTextFragmentLen = 0;
	char* _nextTextFragment = nullptr;
	size_t _realNextTextFragmentLen = 0;
	
	void patternFragmentSiftIteration(const uint iPatternFragment, std::vector<std::vector<EntryPair> >& entries);
	
	void filterEntriesOfTextFragment(std::vector<EntryPair>& entriesOfTextFragment,
									 std::vector<EntryPair>& crtFilteredEntries,
									 std::vector<EntryPair>& nextFilteredEntries,
									 const uint iPatternFragment,
									 const uint iTextFragment);
	
	void filterEntry(const EntryPair& pair,
					 const uint iPatternFragment,
					 std::vector<EntryPair>& crtFilteredEntries,
					 std::vector<EntryPair>& nextFilteredEntries);
	
	bool patternFragmentExistsInTextFragment(const char* patternFragment,
											 const size_t realPatternFragmentLen,
											 const char* textFragment,
											 const size_t realTextFragmentLen,
											 const uint positionInTextFragment);
	
	
};


#endif /* SecondStepSearchInFile_hpp */

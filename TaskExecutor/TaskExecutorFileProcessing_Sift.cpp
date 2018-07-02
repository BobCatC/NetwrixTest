//
//  TaskExecutorFileProcessing_Sift.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 29.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "TaskExecutor.hpp"




/* ---------------------------------------- TaskExecutor siftEntries ------------------------------------- */
/* ---------------------------------------- Each Iteration Processes Each Pattern Fragment --------------- */

void TaskExecutor::siftEntries(std::vector<std::vector<EntryPair> > &entries)
{
	for(uint iPatternFragment = 1; iPatternFragment < _patternMetrics.numberOfFragments; ++iPatternFragment) {
		
		patternFragmentSiftIteration(iPatternFragment, entries);
	}
}


/* ---------------------------------------- TaskExecutor patternFragmentSiftIteration ------------------------------ */
/* ---------------------------------------- Firstly We Download Current Pattern Fragment --------------------------- */
/* ---------------------------------------- In Each Iteration Have Downloaded Current And Next Text Fragment ------- */

void TaskExecutor::patternFragmentSiftIteration(const uint iPatternFragment, std::vector<std::vector<EntryPair> >& entries)
{
	size_t realPatternFragmentLen;
	size_t realCrtTextFragmentLen, realNextTextFragmentLen;
	
	realPatternFragmentLen = getRealPatternFragmentLen(iPatternFragment);
	realNextTextFragmentLen = getRealTextFragmentLen(0);
	
	// Pattern fragment won't be changed
	char* crtPatternFragment = _buffers.buf;
	downloadFragment(_patternFile, iPatternFragment * _patternMetrics.fragmentLen, realPatternFragmentLen, crtPatternFragment);
	
	
	// We'll go through all the text
	// And we'll always have memory mapping of text of current frgment and next fragment
	char* crtTextFragment, *nextTextFragment;
	crtTextFragment = _buffers.buf + _patternMetrics.fragmentLen + 1;
	nextTextFragment = _buffers.buf + _patternMetrics.fragmentLen + 2 + _textMetrics.fragmentLen;
	
	downloadFragment(_textFile, 0, realNextTextFragmentLen, nextTextFragment);
	
	std::vector<EntryPair> crtFilteredEntries, nextFilteredEntries;

	
	for(uint iTextFragment = 0; iTextFragment < _textMetrics.numberOfFragments; ++iTextFragment) {
		
		// We get current text fragment from next one and download new next
		std::swap(realCrtTextFragmentLen, realNextTextFragmentLen);
		std::swap(crtTextFragment, nextTextFragment);
		
		realNextTextFragmentLen = getRealTextFragmentLen(iTextFragment + 1);
		
		downloadFragment(_textFile,
						 (iTextFragment + 1) * (_textMetrics.fragmentLen - _patternMetrics.fragmentLen),
						 realNextTextFragmentLen,
						 nextTextFragment);
		
		
		filterEntriesOfTextFragment(entries[iTextFragment],
									crtFilteredEntries,
									nextFilteredEntries,
									iPatternFragment,
									iTextFragment,
									crtPatternFragment,
									realPatternFragmentLen,
									crtTextFragment,
									realCrtTextFragmentLen,
									nextTextFragment,
									realNextTextFragmentLen);
		
	}
	
}


/* ---------------------------------------- TaskExecutor filterEntriesOfTextFragment ------------------------------ */
/* ---------------------------------------- Checks All Entries In Current Text Fragment --------------------------- */

void TaskExecutor::filterEntriesOfTextFragment(std::vector<EntryPair>& entriesOfTextFragment,
											   std::vector<EntryPair>& crtFilteredEntries,
											   std::vector<EntryPair>& nextFilteredEntries,
											   const uint iPatternFragment,
											   const uint iTextFragment,
											   const char* crtPatternFragment,
											   const size_t realPatternFragmentLen,
											   const char* crtTextFragment,
											   const size_t realCrtTextFragmentLen,
											   const char* nextTextFragment,
											   const size_t realNextTextFragmentLen)
{
	// As we pass through the whole vector, it's much better to fill new vector, than to change current
//	std::vector<EntryPair> filteredEntries;
//	filteredEntries.reserve(entriesOfTextFragment.size());
	crtFilteredEntries.swap(nextFilteredEntries);
	nextFilteredEntries.clear();
	
	for(const EntryPair& pair : entriesOfTextFragment) {
		
		filterEntry(pair,
					iPatternFragment,
					crtPatternFragment,
					realPatternFragmentLen,
					crtTextFragment,
					realCrtTextFragmentLen,
					nextTextFragment,
					realNextTextFragmentLen,
					crtFilteredEntries,
					nextFilteredEntries);
		
	}
	
	entriesOfTextFragment = crtFilteredEntries;
	
}


/* ---------------------------------------- TaskExecutor filterEntry ----------------------------------------------- */
/* ---------------------------------------- Checks, If Current Pattern Fragment Can Be Continuation Of Current Entry */
/* ---------------------------------------- And If It Can, It Increases pair.second And Pushes It Back ------------- */

void TaskExecutor::filterEntry(const EntryPair& pair,
							   const uint iPatternFragment,
							   const char* crtPatternFragment,
							   const size_t realPatternFragmentLen,
							   const char* crtTextFragment,
							   const size_t realCrtTextFragmentLen,
							   const char* nextTextFragment,
							   const size_t realNextTextFragmentLen,
							   std::vector<EntryPair>& crtFilteredEntries,
							   std::vector<EntryPair>& nextFilteredEntries)
{
	const PatternStartPosition startPositionOfPatternFragment = pair.second;
	
	
	uint positionOfNextPatternFragment = (uint)(startPositionOfPatternFragment + _patternMetrics.fragmentLen);
	
	const char* textFragment;
	size_t textFragmentLen;
	std::vector<EntryPair>* resultEntries = nullptr;
	
	// Maybe pattern fragment we'll be not in current text fragment, but in next
	if(positionOfNextPatternFragment < _textMetrics.fragmentLen) {
		
		textFragment = crtTextFragment;
		textFragmentLen = realCrtTextFragmentLen;
		resultEntries = &crtFilteredEntries;
	}
	else {
		textFragment = nextTextFragment;
		textFragmentLen = realNextTextFragmentLen;
		positionOfNextPatternFragment -= _textMetrics.fragmentLen;
		resultEntries = &nextFilteredEntries;
	}
	
	// Checks, if bytes in text fragment and bytes int pattern fragment are equal
	if(patternFragmentExistsInTextFragment(crtPatternFragment, realPatternFragmentLen, textFragment, textFragmentLen, positionOfNextPatternFragment))
	{
		EntryPair newPair(pair.first,
						  ( iPatternFragment == (_patternMetrics.numberOfFragments - 1) ) ? pair.second : positionOfNextPatternFragment);
		
		resultEntries->push_back(newPair);
	}
	
}


/* ---------------------------------------- TaskExecutor patternFragmentExistsInTextFragment ------------------------------ */
/* ---------------------------------------- Checks If Pattern Fragment Exists In Text Fragment On Position ---------------- */

bool TaskExecutor::patternFragmentExistsInTextFragment(const char* patternFragment,
													   const size_t realPatternFragmentLen,
													   const char* textFragment,
													   const size_t realTextFragmentLen,
													   const uint positionInTextFragment)
{
	if(realPatternFragmentLen > realTextFragmentLen - positionInTextFragment) {
		return false;
	}
	
	return strncmp(patternFragment, textFragment + positionInTextFragment, realPatternFragmentLen) == 0;
}













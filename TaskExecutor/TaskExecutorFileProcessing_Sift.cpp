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
	for(size_t iPatternFragment = 1; iPatternFragment < _numberOfFragmentsOfPattern; ++iPatternFragment) {
		
		patternFragmentSiftIteration(iPatternFragment, entries);
	}
}


/* ---------------------------------------- TaskExecutor patternFragmentSiftIteration ------------------------------ */
/* ---------------------------------------- In Each Iteration Firstly We Filter Entries In Each Text Fragment ------ */
/* ---------------------------------------- And Increase Entries For Next Iteration -------------------------------- */

void TaskExecutor::patternFragmentSiftIteration(const size_t iPatternFragment, std::vector<std::vector<EntryPair> >& entries)
{
	size_t realPatternFragmentLen;
	size_t realCrtTextFragmentLen, realNextTextFragmentLen;
	
	realPatternFragmentLen = getRealPatternFragmentLen(iPatternFragment);
	realNextTextFragmentLen = getRealTextFragmentLen(0);
	
	// Pattern fragment won't be changed
	char* crtPatternFragment = _buf;
	downloadFragment(_patternFile, iPatternFragment * _patternFragmentLen, realPatternFragmentLen, crtPatternFragment);
	
	
	// We'll go through all the text
	// And we'll always have memory mapping of text of current frgment and next fragment
	char* crtTextFragment, *nextTextFragment;
	crtTextFragment = _buf + _patternFragmentLen + 1;
	nextTextFragment = _buf + _patternFragmentLen + 2 + _textFragmentLen;
	
	downloadFragment(_textFile, 0, realNextTextFragmentLen, nextTextFragment);
	
	std::vector<EntryPair> crtFilteredEntries, nextFilteredEntries;

	
	for(int iTextFragment = 0; iTextFragment < _numberOfFragmentsOfTextWithSuperimposition; ++iTextFragment) {
		
		// We get current text fragment from next one and download new next
		std::swap(realCrtTextFragmentLen, realNextTextFragmentLen);
		std::swap(crtTextFragment, nextTextFragment);
		
		realNextTextFragmentLen = getRealTextFragmentLen(iTextFragment + 1);
		
		downloadFragment(_textFile,
						 (iTextFragment + 1) * (_textFragmentLen - _patternFragmentLen),
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
											   const size_t iPatternFragment,
											   const size_t iTextFragment,
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

void TaskExecutor::filterEntry(const EntryPair& pair,
							   const size_t iPatternFragment,
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
	
	
	
	
	size_t positionOfNextPatternFragment = startPositionOfPatternFragment + _patternFragmentLen;
	
	const char* textFragment;
	size_t textFragmentLen;
	std::vector<EntryPair>* resultEntries = nullptr;
	
	// Maybe pattern fragment we'll be not in current text fragment, but in next
	if(positionOfNextPatternFragment < _textFragmentWithSuperimpositionLen) {
		
		textFragment = crtTextFragment;
		textFragmentLen = realCrtTextFragmentLen;
		resultEntries = &crtFilteredEntries;
	}
	else {
		textFragment = nextTextFragment;
		textFragmentLen = realNextTextFragmentLen;
		positionOfNextPatternFragment -= _textFragmentWithSuperimpositionLen;
		resultEntries = &nextFilteredEntries;
	}
	
	// Checks, if bytes in text fragment and bytes int pattern fragment are equal
	if(patternFragmentExistsInTextFragment(crtPatternFragment, realPatternFragmentLen, textFragment, textFragmentLen, positionOfNextPatternFragment))
	{
		EntryPair newPair(pair.first,
						  ( iPatternFragment == (_numberOfFragmentsOfPattern - 1) ) ? pair.second : positionOfNextPatternFragment);
		
		resultEntries->push_back(newPair);
	}
	
}


/* ---------------------------------------- TaskExecutor patternFragmentExistsInTextFragment ------------------------------ */
/* ---------------------------------------- Checks If Pattern Fragment Exists In Text Fragment On Position ---------------- */

bool TaskExecutor::patternFragmentExistsInTextFragment(const char* patternFragment,
													   const size_t realPatternFragmentLen,
													   const char* textFragment,
													   const size_t realTextFragmentLen,
													   const size_t positionInTextFragment)
{
	if(realPatternFragmentLen > realTextFragmentLen - positionInTextFragment) {
		return false;
	}
	
	return strncmp(patternFragment, textFragment + positionInTextFragment, realPatternFragmentLen) == 0;
}













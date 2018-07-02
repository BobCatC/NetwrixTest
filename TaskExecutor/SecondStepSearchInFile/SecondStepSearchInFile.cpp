//
//  SecondStepSearchInFile.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 02.07.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include "SecondStepSearchInFile.hpp"


/* ---------------------------------------- SecondStepSearchInFile constructor ------------------------ */
/* ---------------------------------------- Recieves Initial Data ------------------------------------- */

SecondStepSearchInFile::SecondStepSearchInFile(const BuffersForSearch& buffers,
											   const PatternMetrics& patternMetrics,
											   const TextMetrics& textMetrics,
											   FILE* patternFile,
											   FILE* textFile) :
_buffers(buffers),
_patternMetrics(patternMetrics),
_textMetrics(textMetrics),
_patternFile(patternFile),
_textFile(textFile)
{ }


/* ---------------------------------------- SecondStepSearchInFile siftEntries --------------------------- */
/* ---------------------------------------- Each Iteration Processes Each Pattern Fragment --------------- */


void SecondStepSearchInFile::siftEntries(std::vector<std::vector<EntryPair>>& entries)
{
	for(uint iPatternFragment = 1; iPatternFragment < _patternMetrics.numberOfFragments; ++iPatternFragment) {
		
		patternFragmentSiftIteration(iPatternFragment, entries);
	}
}

/* ---------------------------------------- SecondStepSearchInFile patternFragmentSiftIteration -------------------- */
/* ---------------------------------------- Firstly We Download Current Pattern Fragment --------------------------- */
/* ---------------------------------------- In Each Iteration Have Downloaded Current And Next Text Fragment ------- */

void SecondStepSearchInFile::patternFragmentSiftIteration(const uint iPatternFragment, std::vector<std::vector<EntryPair> >& entries)
{
	_realPatternFragmentLen = _patternMetrics.getRealFragmentLen(iPatternFragment);
	_realNextTextFragmentLen = _textMetrics.getRealFragmentLen(0);
	
	// Pattern fragment won't be changed
	_crtPatternFragment = _buffers.buf;
	downloadFragment(_patternFile, iPatternFragment * _patternMetrics.fragmentLen, _realPatternFragmentLen, _crtPatternFragment);
	
	
	// We'll go through all the text
	// And we'll always have memory mapping of text of current frgment and next fragment
	_crtTextFragment = _buffers.buf + _patternMetrics.fragmentLen + 1;
	_nextTextFragment = _buffers.buf + _patternMetrics.fragmentLen + 2 + _textMetrics.fragmentWithoutSuperimpositionLen;
	
	downloadFragment(_textFile, 0, _realNextTextFragmentLen, _nextTextFragment);
	
	std::vector<EntryPair> crtFilteredEntries, nextFilteredEntries;
	
	
	for(uint iTextFragment = 0; iTextFragment < _textMetrics.numberOfFragments; ++iTextFragment) {
		
		// We get current text fragment from next one and download new next
		std::swap(_realCrtTextFragmentLen, _realNextTextFragmentLen);
		std::swap(_crtTextFragment, _nextTextFragment);
		
		_realNextTextFragmentLen = _textMetrics.getRealFragmentLen(iTextFragment + 1);
		
		downloadFragment(_textFile,
						 (iTextFragment + 1) * _textMetrics.fragmentWithSuperimpositionLen,
						 _realNextTextFragmentLen,
						 _nextTextFragment);
		
		
		filterEntriesOfTextFragment(entries[iTextFragment],
									crtFilteredEntries,
									nextFilteredEntries,
									iPatternFragment,
									iTextFragment);
		
	}
	
}


/* ---------------------------------------- SecondStepSearchInFile filterEntriesOfTextFragment -------------------- */
/* ---------------------------------------- Checks All Entries In Current Text Fragment --------------------------- */

void SecondStepSearchInFile::filterEntriesOfTextFragment(std::vector<EntryPair>& entriesOfTextFragment,
											   std::vector<EntryPair>& crtFilteredEntries,
											   std::vector<EntryPair>& nextFilteredEntries,
											   const uint iPatternFragment,
											   const uint iTextFragment)
{
	// As we pass through the whole vector, it's much better to fill new vector, than to change current
	//	std::vector<EntryPair> filteredEntries;
	//	filteredEntries.reserve(entriesOfTextFragment.size());
	crtFilteredEntries.swap(nextFilteredEntries);
	nextFilteredEntries.clear();
	
	for(const EntryPair& pair : entriesOfTextFragment) {
		
		filterEntry(pair,
					iPatternFragment,
					crtFilteredEntries,
					nextFilteredEntries);
		
	}
	
	entriesOfTextFragment = crtFilteredEntries;
	
}


/* ---------------------------------------- SecondStepSearchInFile filterEntry ------------------------------------- */
/* ---------------------------------------- Checks, If Current Pattern Fragment Can Be Continuation Of Current Entry */
/* ---------------------------------------- And If It Can, It Increases pair.second And Pushes It Back ------------- */

void SecondStepSearchInFile::filterEntry(const EntryPair& pair,
							   const uint iPatternFragment,
							   std::vector<EntryPair>& crtFilteredEntries,
							   std::vector<EntryPair>& nextFilteredEntries)
{
	const PatternStartPosition startPositionOfPatternFragment = pair.second;
	
	
	uint positionOfNextPatternFragment = (uint)(startPositionOfPatternFragment + _patternMetrics.fragmentLen);
	
	const char* textFragment;
	size_t textFragmentLen;
	std::vector<EntryPair>* resultEntries = nullptr;
	
	// Maybe pattern fragment we'll be not in current text fragment, but in next
	if(positionOfNextPatternFragment < _textMetrics.fragmentWithSuperimpositionLen) {
		
		textFragment = _crtTextFragment;
		textFragmentLen = _realCrtTextFragmentLen;
		resultEntries = &crtFilteredEntries;
	}
	else {
		textFragment = _nextTextFragment;
		textFragmentLen = _realNextTextFragmentLen;
		positionOfNextPatternFragment -= _textMetrics.fragmentWithSuperimpositionLen;
		resultEntries = &nextFilteredEntries;
	}
	
	// Checks, if bytes in text fragment and bytes int pattern fragment are equal
	if(patternFragmentExistsInTextFragment(_crtPatternFragment,
										   _realPatternFragmentLen,
										   textFragment,
										   textFragmentLen,
										   positionOfNextPatternFragment)) {
		
		const CrtFragmentStartPosition crtPosition = iPatternFragment == (_patternMetrics.numberOfFragments - 1) ? pair.second : positionOfNextPatternFragment;
		EntryPair newPair(pair.first, crtPosition);
		
		resultEntries->push_back(newPair);
	}
	
}


/* ---------------------------------------- SecondStepSearchInFile patternFragmentExistsInTextFragment -------------------- */
/* ---------------------------------------- Checks If Pattern Fragment Exists In Text Fragment On Position ---------------- */

bool SecondStepSearchInFile::patternFragmentExistsInTextFragment(const char* patternFragment,
													   const size_t realPatternFragmentLen,
													   const char* textFragment,
													   const size_t realTextFragmentLen,
													   const uint positionInTextFragment)
{
	if(realPatternFragmentLen > realTextFragmentLen - positionInTextFragment) {
		return false;
	}
	
	return 0 == strncmp(patternFragment, textFragment + positionInTextFragment, realPatternFragmentLen);
}

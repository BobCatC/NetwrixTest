//
//  FirstStepSearchInFile.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 02.07.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "FirstStepSearchInFile.hpp"
#include "../../FileSystem/FileSystem.hpp"

FirstStepSearchInFile::FirstStepSearchInFile(const BuffersForSearch& buffers,
											 const PatternMetrics& patternMetrics,
											 const TextMetrics& textMetrics,
											 FILE* patternFile,
											 FILE* piForFirstPatternFragmentFile,
											 FILE* textFile) :
_buffers(buffers),
_patternMetrics(patternMetrics),
_textMetrics(textMetrics),
_patternFile(patternFile),
_piForFirstPatternFragmentFile(piForFirstPatternFragmentFile),
_textFile(textFile)
{ }




void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from);



/* ---------------------------------------- TaskExecutor findEntriesOfFirstFragment ------------------------------ */
/* ---------------------------------------- Findes All The Entries Of First Pattern Fragment In All Text Fragments */

void FirstStepSearchInFile::findEntriesOfFirstFragment(std::vector<std::vector<EntryPair>> &entries)
{
	const uint iPatternFragment = 0;
	size_t realPatternFragmentLen;
	realPatternFragmentLen = _patternMetrics.getRealFragmentLen(iPatternFragment);
	
	// Pattern fragment won't be changed => we download it only once
	downloadFragment(_patternFile, 0, realPatternFragmentLen, _buffers.s);
	
	// This is saved value of "pi" for first pattern fragment
	downloadFragment(_piForFirstPatternFragmentFile, 0, _patternMetrics.fragmentLen, _buffers.pi);
	
	for(uint iTextFragment = 0; iTextFragment < _textMetrics.numberOfFragments; ++iTextFragment) {
		
		size_t realTextFragmentLen;
		
		// Last fragment can be less
		realTextFragmentLen = _textMetrics.getRealFragmentLen(iTextFragment);
		
		// Without decrement some "boundary" entries will be found twice (because of superimposition)
		if(iTextFragment != _textMetrics.numberOfFragments - 1)
			--realTextFragmentLen;
		
		
		_buffers.s[realPatternFragmentLen] = '#'; // divider
		
		downloadFragment(_textFile,
						 iTextFragment * _textMetrics.fragmentWithSuperimpositionLen,
						 realTextFragmentLen,
						 _buffers.s + realPatternFragmentLen + 1);
		
		bool nextIterationNeeded =  searchWithPrefixFunc(realPatternFragmentLen,
														 iTextFragment,
														 realPatternFragmentLen + 1 + realTextFragmentLen,
														 entries[iTextFragment]);
		
		if(nextIterationNeeded == false) {
			break;
		}
		
		// Now entries[iTextFragment] contains all the entries of first pattern fragment in iTextFragment-th fragment of the text
	}
}


/* ---------------------------------------- searchWithPrefixFunc ---------------------------------------- */
/* ---------------------------------------- Standart Search With Prefix Function With Little Modification */

bool FirstStepSearchInFile::searchWithPrefixFunc(const size_t realPatternFragmentLen,
										const uint iTextFragment,
										const size_t len,
										std::vector<EntryPair>& result)
{
	// Prefix function will start counting from "realPatternFragmentLen", as "_pi" from Zero to "realPatternFragmentLen" was downloaded
	prefixFunction(_buffers.s, _buffers.pi, len, (int)realPatternFragmentLen);
	
	bool goOnSearching = true;
	size_t i = 2 * realPatternFragmentLen;
	while (i < len) {
		
		if(_buffers.pi[i] >= realPatternFragmentLen) {
			
			{	// Modification.
				// If text or pattern contains symbol '#', which divides them in "_s",
				// whithout this code some entries won't be found.
				size_t j = i;
				while(_buffers.pi[j] > realPatternFragmentLen) {
					j = _buffers.pi[j] - 1;
				}
				_buffers.pi[i] = _buffers.pi[j];
			}
			
			if(_buffers.pi[i] == realPatternFragmentLen) {
				
				CrtFragmentStartPosition positionInCrtTextFragment = (CrtFragmentStartPosition)(i - 2 * realPatternFragmentLen);
				PatternStartPosition absolutePosition = (PatternStartPosition)(positionInCrtTextFragment + (iTextFragment * _textMetrics.fragmentWithSuperimpositionLen));
				EntryPair pair(absolutePosition, positionInCrtTextFragment);
				
				// (First pattern fragment can't be on position, if the whole pattern won't fit it )
				if(firstPatternFragmentCanBeOnPosition(positionInCrtTextFragment, iTextFragment)) {
					result.push_back(pair);
				}
				else {
					goOnSearching = false;
					break;
				}
			}
		}
		
		++i;
	}
	
	return goOnSearching;
}


/* ---------------------------------------- TaskExecutor firstPatternFragmentCanBeOnPosition ------------------------------------- */
/* ---------------------------------------- Counts Rightmost Position Of First Pattern Fragment According To Pattern And Text Size */

bool FirstStepSearchInFile::firstPatternFragmentCanBeOnPosition(const size_t position, const uint iTextFragment)
{
	size_t absolutePosition = position + (iTextFragment * _textMetrics.fragmentWithSuperimpositionLen);
	const size_t maxSizeForPosition = _textMetrics.len - absolutePosition;
	
	return ( _patternMetrics.len <= maxSizeForPosition );
}










/* ======================================== Prefix Function ======================================================================= */

void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from)
{
	for(int i = from; i < len; ++i) {
		int j = pi[i - 1];
		while(j > 0 && s[i] != s[j]) {
			j = pi[j - 1];
		}
		if(s[i] == s[j]) {
			++j;
		}
		pi[i] = j;
	}
}






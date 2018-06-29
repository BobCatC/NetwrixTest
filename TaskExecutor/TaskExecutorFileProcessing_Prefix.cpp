//
//  TaskExecutorFileProcessing_Prefix.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 29.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "TaskExecutor.hpp"

void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from);



/* ---------------------------------------- TaskExecutor findEntriesOfFirstFragment ------------------------------ */
/* ---------------------------------------- Findes All The Entries Of First Pattern Fragment In All Text Fragments */

void TaskExecutor::findEntriesOfFirstFragment(std::vector<std::vector<EntryPair>> &entries)
{
	size_t iPatternFragment = 0;
	size_t realPatternFragmentLen;
	realPatternFragmentLen = getRealPatternFragmentLen(iPatternFragment);
	
	downloadFragment(_patternFile, 0, realPatternFragmentLen, _s);
	
	downloadFragment(_piForFirstPatternFragmentFile, 0, _patternFragmentLen * sizeof(*_pi), (char*)_pi);
	
	for(size_t iTextFragment = 0; iTextFragment < _numberOfFragmentsOfTextWithSuperimposition; ++iTextFragment) {
		
		size_t realTextFragmentLen;
		
		// Last fragment can be less
		realTextFragmentLen = getRealTextFragmentLen(iTextFragment);
		
		// Without decrement some "boundary" entries will be found twice (because of imposition)
		if(iTextFragment != _numberOfFragmentsOfTextWithSuperimposition - 1)
			--realTextFragmentLen;
		
		
		((unsigned char*)_s)[realPatternFragmentLen] = 227; // pi (non-ascii symbol)
		
		downloadFragment(_textFile,
						 iTextFragment * _textFragmentWithSuperimpositionLen,
						 realTextFragmentLen,
						 _s + realPatternFragmentLen + 1);
		
		searchWithPrefixFunc(realPatternFragmentLen,
							 iTextFragment,
							 realPatternFragmentLen + 1 + realTextFragmentLen,
							 entries[iTextFragment]);
		
		// Now entries[iTextFragment] contains all the entries of first pattern fragment in iTextFragment-th fragment of the text
	}
}


/* ---------------------------------------- searchWithPrefixFunc ---------------------------------------- */
/* ---------------------------------------- Standart Search With Prefix Function With Little Modification */

void TaskExecutor::searchWithPrefixFunc(const size_t realPatternFragmentLen,
										const size_t iTextFragment,
										const size_t len,
										std::vector<EntryPair>& result)
{
	prefixFunction(_s, _pi, len, (int)realPatternFragmentLen);
	
	size_t i = 2 * realPatternFragmentLen;
	while (i < len) {
		
		if(_pi[i] >= realPatternFragmentLen) {
			
			{	// Modification.
				// If text or pattern contains symbol '\227', which divides them in "_s",
				// whithout this code some entries won't be found.
				size_t j = i;
				while(_pi[j] > realPatternFragmentLen) {
					j = _pi[j] - 1;
				}
				_pi[i] = _pi[j];
			}
			
			if(_pi[i] == realPatternFragmentLen) {
				
				CrtFragmentEntry position = (CrtFragmentEntry)(i - 2 * realPatternFragmentLen);
				FirstFragmentEntry absolutePosition = (FirstFragmentEntry)(position + (iTextFragment * _textFragmentWithSuperimpositionLen));
				EntryPair pair(absolutePosition, position);
				
				if(firstPatternFragmentCanBeOnPosition(position, iTextFragment)) {
					result.push_back(pair);
				}
			}
		}
		
		++i;
	}
}


/* ---------------------------------------- TaskExecutor firstPatternFragmentCanBeOnPosition ------------------------------------- */
/* ---------------------------------------- Counts Rightmost Position Of First Pattern Fragment According To Pattern And Text Size */

bool TaskExecutor::firstPatternFragmentCanBeOnPosition(size_t position, size_t iTextFragment)
{
	size_t absolutePosition = position + (iTextFragment * _textFragmentWithSuperimpositionLen);
	const size_t maxSizeForPosition = _textLen - absolutePosition;
	
	return ( _patternLen <= maxSizeForPosition );
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
















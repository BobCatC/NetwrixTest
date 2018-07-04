//
//  CommonTypes.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 03.07.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "CommonTypes.hpp"
#include "../../FileSystem/FileSystem.hpp"


/* ---------------------------------------- BuffersForSearch countDefaultMetrics ------------- */
/* ---------------------------------------- length of "pi" and "s" is equal, but size is not   */

void BuffersForSearch::countDefaultMetrics(const size_t cbMaxBufSize)
{
	piArraySize = (cbMaxBufSize * 4) / 5;
	piArrayLen = piArraySize / sizeof(*pi);
	
	sArraySize = (cbMaxBufSize * 1) / 5;
	sArrayLen = sArraySize / sizeof(*s);
}


/* ---------------------------------------- BuffersForSearch initBuffers  ------------------------------ */
/* ---------------------------------------- allocates memory in "buf" and distributes it to "s" and "pi" */

void BuffersForSearch::initBuffers(const size_t cbMaxBufSize, void* pointerPatternMetrics, const std::string& patternFileName)
{
	PatternMetrics& patternMetrics = *( (PatternMetrics*)pointerPatternMetrics );
	
	buf = new char[cbMaxBufSize];
	
	if(buf == nullptr) {
		throw "Memory Allocation Error";
	}
	
	// metrics of "pi" and "s"
	countDefaultMetrics(cbMaxBufSize);
	
	// metrics of pattern (len, fragment len, quontity of framgents)
	patternMetrics.countPatternMetrics(bfs::file_size(bfs::path(patternFileName)), *this);
	
	s = buf;
	pi = (int32_t*)(buf + sArraySize);
}




/* ---------------------------------------- PatternMetrics countPatternMetrics ----------------------------------------------------- */
/* ---------------------------------------- gets the pattern file len, counts pattern fragment len and quontity of pattern fragments */

void PatternMetrics::countPatternMetrics(const size_t lenOfPatternFile, const BuffersForSearch& buffers)
{
	len = lenOfPatternFile;
	fragmentLen = ((buffers.sArrayLen - 1) * 1) / 7;
	
	// if pattern is little, we can set "_patternFragmentLen" to the size of whole pattern
	if(len < fragmentLen) {
		fragmentLen = len;
	}
	
	// round up
	numberOfFragments = (uint)( (len + fragmentLen - 1) / (fragmentLen) );
}


/* ---------------------------------------- Last Fragment Is Less ----------------------------------------------- */

size_t PatternMetrics::getRealFragmentLen(const uint iPatternFragment) const
{
	size_t realPatternFragmentLen;
	if(iPatternFragment == numberOfFragments - 1) {
		realPatternFragmentLen = len - (numberOfFragments - 1) * fragmentLen;
	}
	else {
		realPatternFragmentLen = fragmentLen;
	}
	return realPatternFragmentLen;
	
}





/* ---------------------------------------- TextMetrics countTextMetrics ------------------------------ */
/* ---------------------------------------- does the same as countPatternMetrics ---------------------- */

void TextMetrics::countTextMetrics(const size_t lenOfTextFile, const PatternMetrics& patternMetrics, const BuffersForSearch& buffers)
{
	len = lenOfTextFile;
	fragmentWithoutSuperimpositionLen = buffers.sArraySize - 1 - patternMetrics.fragmentLen;
	
	// every text fragment superimpose on previous one, "delta" of superimposition == "textFragmentWithoutSuperimpositionLen" - "patternFragmentLen"
	fragmentWithSuperimpositionLen = fragmentWithoutSuperimpositionLen - patternMetrics.fragmentLen;
	
	// !!! it's important to count it in signed type
	numberOfFragments = (uint)(   1 +
							   ( ((long)len - (long)patternMetrics.fragmentLen - 1 ) /
								(long)(fragmentWithoutSuperimpositionLen - patternMetrics.fragmentLen) )   );
}


/* ---------------------------------------- Last Fragment Is Not The Same Size --------------------------------- */

size_t TextMetrics::getRealFragmentLen(const uint iTextFragment) const
{
	size_t realTextFragmentLen;
	if(iTextFragment == numberOfFragments - 1) {
		realTextFragmentLen = len - (numberOfFragments - 1) * fragmentWithSuperimpositionLen;
	}
	else {
		realTextFragmentLen = fragmentWithoutSuperimpositionLen;
	}
	return realTextFragmentLen;
}

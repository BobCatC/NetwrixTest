//
//  TaskExecutorFileProcessing.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 29.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "TaskExecutor.hpp"


/* ======================================== TaskExecutor File Processing Functions ================================================ */

/* ---------------------------------------- TaskExecutor processFile -------------------------------------------------------------- */
/* ---------------------------------------- The Main Idea Of Searching is Here: --------------------------------------------------- */
/* ---------------------------------------- Firstly We Find, Where Can Be The First Pattern Fragment ------------------------------ */
/* ---------------------------------------- Then We Chack Each Entry Of First Fragment, If Second Fragment Can Be It's Continuation */
/* ---------------------------------------- If No => We Delete It From Entries ("sift" entries) ----------------------------------- */
/* ---------------------------------------- After Previous Iteration We Have Entries Of Second Pattern Fragment ------------------- */
/* ---------------------------------------- Next Iteration We Do The Same With Second And Third Pattern Fragment ------------------ */

void TaskExecutor::processFile()
{
	countTextMetrics();
	if(_patternMetrics.len > _textMetrics.len){
		return;
	}
	
	_textFile = fopen(_textFilePath.c_str(), "r");
	if(_textFile == nullptr) {
		throw ( "Couldn't open file \"" + _textFilePath + "\"");
	}
	
	// entries[i] - vector of entries of current pattern fragment in i-th text fragment.
	// What is the pattern fragment, it depends on state of searching.
	std::vector<std::vector<EntryPair>> entries(_textMetrics.numberOfFragments);
	
	FirstStepSearchInFile firstStep(_buffers, _patternMetrics, _textMetrics, _patternFile, _piForFirstPatternFragmentFile, _textFile);
	firstStep.findEntriesOfFirstFragment(entries);
	// Now "entries" contain all entries of First pattern fragment
	
	
	SecondStepSearchInFile secondStep(_buffers, _patternMetrics, _textMetrics, _patternFile, _textFile);
	// "entries" will be sifted
	secondStep.siftEntries(entries);
	
	
	// Now "entries" contain the final result of searching
	fillResult(entries);
	
	fclose(_textFile);
	_textFile = nullptr;
}


/* ---------------------------------------- TaskExecutor fillResult ---------------------------- */
/* ---------------------------------------- Taskes All Remaining Entries And Fills Result Vector */

void TaskExecutor::fillResult(const std::vector<std::vector<EntryPair> >& entries)
{
	
	for(uint iTextFragment = 0; iTextFragment < _textMetrics.numberOfFragments; ++iTextFragment) {
		
		for(const auto& pair : entries[iTextFragment]) {
			
			const PatternStartPosition patternStartPosition = pair.first;
			_result.push_back((int)patternStartPosition);
		}
		
	}
}





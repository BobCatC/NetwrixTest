//
//  CommonStructures.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 02.07.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CommonStructures_h
#define CommonStructures_h

#include <vector>
#include <stdio.h>
typedef unsigned int uint;



/*---- type of value, that indicates the position of first pattern fragment */
/*---- (the start of the pattern in the text) ----------------------------- */
typedef unsigned int PatternStartPosition;

/*---- type of value, which incicates the position of CURRENT searchable pattern fragment */
typedef unsigned int CrtFragmentStartPosition;
typedef std::pair<PatternStartPosition, CrtFragmentStartPosition> EntryPair;



struct BuffersForSearch {
	
	/*---- Dynamic memory for loading fragments of files ---------------------- */
	/*---- "s" for prefix function. "s" : [pattern]#[text] -------------------- */
	/*---- "pi" for prefix function. "pi" containts result of prefix function */
	/*---- "buf" is allocated in constructor and deleted in destructor ------- */
	/*---- "s" and "pi" point to "buf" memory (don't have own) ------------- */
	char* buf = nullptr;
	char* s = nullptr;
	int32_t* pi = nullptr;
	
	/*---- As we have too less memory, we have to destribute it ------------------- */
	/*---- 4/5 of "buf" size is for "pi" (as sizeof(int32_t) == 4 * sizeof(char)) */
	/*---- 1/5 of "buf" size is for "s" ----------------------------------------- */
	size_t piArraySize, piArrayLen;
	size_t sArraySize, sArrayLen;
	
	void countDefaultMetrics(const size_t cbMaxBufSize);
	
	
	void initBuffers(const size_t cbMaxBufSize, void* pointerPatternMetrics, const std::string& patternFileName);
};

struct PatternMetrics {
	size_t len;
	size_t fragmentLen;
	uint numberOfFragments;
	
	
	void countPatternMetrics(const size_t length, const BuffersForSearch& buffers);
	
	size_t getRealFragmentLen(const uint iPatternFragment) const;
};


struct TextMetrics {
	size_t len;
	size_t fragmentWithoutSuperimpositionLen;
	size_t fragmentWithSuperimpositionLen;
	uint numberOfFragments;
	

	
	void countTextMetrics(const size_t lenOfTextFile, const PatternMetrics& patternMetrics, const BuffersForSearch& buffers);

	
	size_t getRealFragmentLen(const uint iTextFragment) const;
};



#endif /* CommonStructures_h */





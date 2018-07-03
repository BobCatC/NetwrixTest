//
//  CommonTypes.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 03.07.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "CommonTypes.hpp"


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

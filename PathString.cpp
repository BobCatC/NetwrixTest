//
//  CrossPlatformPathString.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 27.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "PathString.hpp"

StdString getStringFromNative(const NativeString& ns) {
	
#if defined(_WIN32) || defined(_WIN64)
	
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	const auto str = converter.to_bytes(ws.c_str());
	return str;
	
#else
	return ns;
#endif
}


//
//  CrossPlatformPathString.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 27.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "PathString.hpp"

const char* getCString(const std::string& s) {
	return s.c_str();
}

const char* getCString(const std::wstring& ws) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	
	return converter.to_bytes(ws.c_str()).c_str();
}

const PathString& getStringOfPath(const bfs::path& path) {
	
#ifdef __APPLE__
	return path.string();
#else
	return path.wstring();
#endif
	
}

PathString getPathStringFromCString(const char* s) {

#ifdef __APPLE
	return PathString(s);
#else
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	return converter.from_bytes(s);
#endif
}
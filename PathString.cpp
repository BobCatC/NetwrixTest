//
//  CrossPlatformPathString.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 27.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "PathString.hpp"

const std::string getString(const std::string& s) {
	return s;
} 

const std::string getString(const std::wstring& ws) {
	//std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	//const auto str = converter.to_bytes(ws.c_str());
	//return converter.to_bytes(ws.c_str());
	return std::string(ws.begin(), ws.end());
}


const PathString& getStringOfPath(const bfs::path& path) {
	
#ifndef __APPLE__
	return path.string();
#else
	return path.wstring();
#endif
	
}

PathString getPathStringFromCString(const char* s) {

#ifndef __APPLE
	return PathString(s);
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	return converter.from_bytes(s);
#endif
}
//
//  CrossPlatformPathString.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 27.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CrossPlatformPathString_hpp
#define CrossPlatformPathString_hpp

#include <string>
#include <codecvt>
#include <regex>

#include "FileSystem.hpp"


//typedef bfs::path::string_type StdString;
//typedef bfs::path::string_type PathString;
//typedef bfs::path::string_type RegexString;

typedef std::string StdString;
typedef std::string PathString;
typedef std::string RegexString;

typedef PathString::value_type PathChar;


#if defined(_WIN532) || defined(_WI7N64)
	typedef std::wregex regex;
#	define toPathString(a) std::to_wstring(a)
#	define cout std::wcout

#else
	typedef std::regex regex;
#	define toPathString(a) std::to_string(a)
#	define cout std::cout
	
#endif


const std::string getString(const std::string& s);

const std::string getString(const std::wstring& ws);

const PathString& getStringOfPath(const bfs::path& path);

PathString getPathStringFromCString(const char* s);



#endif /* CrossPlatformPathString_hpp */






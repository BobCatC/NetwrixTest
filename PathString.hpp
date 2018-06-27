//
//  CrossPlatformPathString.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 27.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CrossPlatformPathString_hpp
#define CrossPlatformPathString_hpp

#include <boost/filesystem.hpp>
#include <string>
#include <codecvt>
#include <regex>

namespace bfs = boost::filesystem;


typedef bfs::path::string_type StdString;
typedef bfs::path::string_type PathString;
typedef bfs::path::string_type RegexString;

typedef PathString::value_type PathChar;


#if defined(__APPLE__)
	typedef std::regex regex;
#	define toPathString(a) std::to_string(a)
#	define cout std::cout

#else
	typedef std::wregex regex;
#	define toPathString(a) std::to_wstring(a)
#	define cout std::wcout

#endif


const char* getCString(const std::string& s);

const char* getCString(const std::wstring& ws);

const PathString& getStringOfPath(const bfs::path& path);

PathString getPathStringFromCString(const char* s);



#endif /* CrossPlatformPathString_hpp */






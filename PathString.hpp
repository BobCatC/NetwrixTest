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

typedef std::string StdString;
typedef bfs::path::string_type NativeString;

StdString getStringFromNative(const NativeString& s);


#endif /* CrossPlatformPathString_hpp */






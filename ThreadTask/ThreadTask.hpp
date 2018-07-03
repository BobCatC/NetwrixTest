//
//  ThreadTask.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef CThreadTask_hpp
#define CThreadTask_hpp


#include "../FileSystem/FileSystem.hpp"


class ThreadTask {
	
public:
	
	ThreadTask(const bfs::path& bfsPath = bfs::path());
	
	
	const std::string getFileNativeName() const;
	const std::string getFilePath() const;
	const bfs::path& getFileBfsPath() const;
	
private:
	bfs::path _bfsPath;
};




#endif /* CThreadTask_hpp */

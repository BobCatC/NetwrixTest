//
//  TaskExecutor.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#ifndef TaskExecutor_hpp
#define TaskExecutor_hpp

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <stdio.h>

#include "../FileSystem/FileSystem.hpp"
#include "../ThreadTask/ThreadTask.hpp"


extern size_t numberOfFiles;
extern size_t fileDoneSize;

/*---- type of value, that indicates the position of first pattern fragment */
/*---- (the start of the pattern in the text) ----------------------------- */
typedef unsigned int PatternStartPosition;

/*---- type of value, which incicates the position of CURRENT searchable pattern fragment */
typedef unsigned int CrtFragmentStartPosition;
typedef std::pair<PatternStartPosition, CrtFragmentStartPosition> EntryPair;




class TaskExecutor {

public:
	
	TaskExecutor(const unsigned int threadID,
				 const std::string& thisThreadOutputFileName,
				 const size_t cbMaxBufSize,
				 const std::string& outputFileDirectory,
				 const std::string& patternFileName,
				 const std::regex& regexMask);
	
	TaskExecutor(const TaskExecutor& exe) = delete;
	TaskExecutor(TaskExecutor& exe) = delete;

	void init();
	
	~TaskExecutor();
	
	
	void doTask(const ThreadTask& task,
				std::vector<std::string>& newTasksFiles,
				std::vector<std::string>& newTasksDirectories);
	
	
private:
	/*---- Initial thread data */
	const unsigned int _threadID;
	const std::string& _outputFileName;
	const size_t _cbMaxBufSize;
	const std::string& _outputFileDirectory;
	const std::string& _patternFileName;
	const std::regex& _regexMask;

	/*---- Pointers at files ------------------------------------------------------------------------- */
	/*---- "_patternFile", "_thisThreadOutputFile" and "_piForFirst..." are always opened (closed in destructor) */
	/*---- "_textFile" is opened only while file processing ------------------------------------------ */
	FILE* _patternFile = nullptr;
	FILE* _piForFirstPatternFragmentFile = nullptr;
	FILE* _textFile = nullptr;
	FILE* _thisThreadOutputFile = nullptr;
	
	/*---- As we always count Prefix Function for first pattern fragment and different text fragment, */
	/*---- we can save the value of "_pi" for first pattern fragment in file and just updoal it ----- */
	std::string _piForFirstPatternFragmentFileName;
	
	
	bfs::path _textFileBfsPath;
	std::string _textFilePath;
	
	
	
	struct BuffersForSearch {
		
		char* buf = nullptr;
		char* s = nullptr;
		int32_t* pi = nullptr;
		
		
		size_t piArraySize, piArrayLen;
		size_t sArraySize, sArrayLen;
	};
	
	/*---- Dynamic memory for loading fragments of files ---------------------- */
	/*---- "_s" for prefix function. _s : [pattern]#[text] -------------------- */
	/*---- "_pi" for prefix function. "_pi" containts result of prefix function */
	/*---- "_buf" is allocated in constructor and deleted in destructor ------- */
	/*---- "_s" and "_pi" point to "_buf" memory (don't have own) ------------- */
	
	
	/*---- As we have too less memory, we have to destribute it ------------------- */
	/*---- 4/5 of "_buf" size is for "_pi" (as sizeof(int32_t) == 4 * sizeof(char)) */
	/*---- 1/5 of "_buf" size is for "_s" ----------------------------------------- */
	BuffersForSearch _buffers;
	
	
	
	struct FileMetrics {
		size_t len;
		size_t fragmentLen;
		uint numberOfFragments;
	};
	
	
	/*---- Both the pattern and the text can't be loaded to memory (full file) ------ */
	/*---- We have to count the size of fragments (it depends on size of "_s" buffer) */
	/*---- And count how many fragments we'll have ---------------------------------- */
	
	FileMetrics _patternMetrics;
	
	/*---- The same with the text --------------------------------------------------------------------- */
	/*---- But to find the pattern correctly in the text we have to impose text fragments on each other */
	/*---- (pattern fragment entry can be at the junction of the text fragments, so we have to impose)  */
	size_t _textFragmentWithoutSuperimpositionLen;
	FileMetrics _textMetrics;
	
	
	std::vector<PatternStartPosition> _result;
	
	
	void openDefaultFiles();
	void initBuffers();
	void countPiForFirstPatternFragment();
	void savePiForFirstPatternFragment();
	
	void countDefaultMetrics();
	void countPatternMetrics();
	void countTextMetrics();
	
	void processDirectory(std::vector<std::string> &newTasksFiles,
						  std::vector<std::string> &newTasksDirectories);
	
	void destributeNewPath(const bfs::path& newPath,
						   std::vector<std::string> &newTasksFiles,
						   std::vector<std::string> &newTasksDirectories);
	
	
	/*================== File Searching */
	void processFile();
	
	
	/* ----------------- Searching First Pattern Fragment */
	
	void findEntriesOfFirstFragment(std::vector<std::vector<EntryPair>>& entries);

	void searchWithPrefixFunc(const size_t realPatternFragmentLen,
							  const uint iTextFragment,
							  const size_t len,
							  std::vector<EntryPair>& result);
	
	bool firstPatternFragmentCanBeOnPosition(const size_t position,
											 const uint iTextFragment);
	
	bool patternFragmentExistsInTextFragment(const char* patternFragment,
											 const size_t realPatternFragmentLen,
											 const char* textFragment,
											 const size_t realTextFragmentLen,
											 const uint positionInTextFragment);
	
	
	/* ----------------- Sifning Found Entries */
	
	void siftEntries(std::vector<std::vector<EntryPair>>& entries);
	
	void patternFragmentSiftIteration(const uint iPatternFragment, std::vector<std::vector<EntryPair> >& entries);
	
	void filterEntriesOfTextFragment(std::vector<EntryPair>& entriesOfTextFragment,
									 std::vector<EntryPair>& crtFilteredEntries,
									 std::vector<EntryPair>& nextFilteredEntries,
									 const uint iPatternFragment,
									 const uint iTextFragment,
									 const char* crtPatternFragment,
									 const size_t realPatternFragmentLen,
									 const char* crtTextFragment,
									 const size_t realCrtTextFragmentLen,
									 const char* nextTextFragment,
									 const size_t realNextTextFragmentLen);
	
	void filterEntry(const EntryPair& pair,
					 const uint iPatternFragment,
					 const char* crtPatternFragment,
					 const size_t realPatternFragmentLen,
					 const char* crtTextFragment,
					 const size_t realCrtTextFragmentLen,
					 const char* nextTextFragment,
					 const size_t realNextTextFragmentLen,
					 std::vector<EntryPair>& crtFilteredEntries,
					 std::vector<EntryPair>& nextFilteredEntries);
	
	
	void fillResult(const std::vector<std::vector<EntryPair>>& entries);
	
	size_t getRealPatternFragmentLen(const uint iPatternFragment);
	size_t getRealTextFragmentLen(const uint iTextFragment);
		
	
	void printResultToFile();
};


#endif /* TaskExecutor_hpp */

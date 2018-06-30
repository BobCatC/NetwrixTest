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
				 const std::string& outputFileName,
				 const size_t cbMaxBufSize,
				 const std::string& outputFileDirectory,
				 const std::string& patternFileName,
				 const std::regex& regexMask);
	
	TaskExecutor(const TaskExecutor& exe) = delete;
	TaskExecutor(TaskExecutor& exe) = delete;

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
	/*---- "_patternFile", "_outputFile" and "_piForFirst..." are always opened (closed in destructor) */
	/*---- "_textFile" is opened only while file processing ------------------------------------------ */
	FILE* _patternFile = nullptr;
	FILE* _piForFirstPatternFragmentFile = nullptr;
	FILE* _textFile = nullptr;
	FILE* _outputFile = nullptr;
	
	/*---- As we always count Prefix Function for first pattern fragment and different text fragment, */
	/*---- we can save the value of "_pi" for first pattern fragment in file and just updoal it ----- */
	std::string _piForFirstPatternFragmentFileName;
	
	
	bfs::path _bfsTextFilePath;
	std::string _textFileNativeName;
	std::string _textFilePath;
	
	/*---- Dynamic memory for loading fragments of files ---------------------- */
	/*---- "_s" for prefix function. _s : [pattern]#[text] -------------------- */
	/*---- "_pi" for prefix function. "_pi" containts result of prefix function */
	/*---- "_buf" is allocated in constructor and deleted in destructor ------- */
	/*---- "_s" and "_pi" point to "_buf" memory (don't have own) ------------- */
	char* _buf;
	char* _s;
	int32_t* _pi = nullptr;

	/*---- As we have too less memory, we have to destribute it ------------------- */
	/*---- 4/5 of "_buf" size is for "_pi" (as sizeof(int32_t) == 4 * sizeof(char)) */
	/*---- 1/5 of "_buf" size is for "_s" ----------------------------------------- */
	size_t _piArraySize, _piArrayLen;
	size_t _sArraySize, _sArrayLen;
	
	/*---- Both the pattern and the text can't be loaded to memory (full file) ------ */
	/*---- We have to count the size of fragments (it depends on size of "_s" buffer) */
	/*---- And count how many fragments we'll have ---------------------------------- */
	size_t _patternLen;
	size_t _patternFragmentLen;
	size_t _numberOfFragmentsOfPattern;
	
	/*---- The same with the text --------------------------------------------------------------------- */
	/*---- But to find the pattern correctly in the text we have to impose text fragments on each other */
	/*---- (pattern fragment entry can be at the junction of the text fragments, so we have to impose)  */
	size_t _textLen;
	size_t _textFragmentLen;
	size_t _textFragmentWithSuperimpositionLen;
	size_t _numberOfFragmentsOfTextWithSuperimposition;
	
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
							  const size_t iTextFragment,
							  const size_t len,
							  std::vector<EntryPair>& result);
	
	bool firstPatternFragmentCanBeOnPosition(const size_t position,
											 const size_t iTextFragment);
	
	bool patternFragmentExistsInTextFragment(const char* patternFragment,
											 const size_t realPatternFragmentLen,
											 const char* textFragment,
											 const size_t realTextFragmentLen,
											 const size_t positionInTextFragment);
	
	
	/* ----------------- Sifning Found Entries */
	
	void siftEntries(std::vector<std::vector<EntryPair>>& entries);
	
	void patternFragmentSiftIteration(const size_t iPatternFragment, std::vector<std::vector<EntryPair> >& entries);
	
	void filterEntriesOfTextFragment(std::vector<EntryPair>& entriesOfTextFragment,
									 const size_t iTextFragment,
									 const char* crtPatternFragment,
									 const size_t realPatternFragmentLen,
									 const char* crtTextFragment,
									 const size_t realCrtTextFragmentLen,
									 const char* nextTextFragment,
									 const size_t realNextTextFragmentLen);
	
	void filterEntry(const EntryPair& pair,
					 const char* crtPatternFragment,
					 const size_t realPatternFragmentLen,
					 const char* crtTextFragment,
					 const size_t realCrtTextFragmentLen,
					 const char* nextTextFragment,
					 const size_t realNextTextFragmentLen,
					 std::vector<EntryPair>& filteredEntries);
	
	void increasePositions(std::vector<std::vector<EntryPair>>& entries);
	
	void increaseEntry(const EntryPair& pair,
					   size_t iTextFragment,
					   std::vector<EntryPair>& filteredEntriesCrt,
					   std::vector<EntryPair>& filteredEntriesNext);
	
	
	
	void fillResult(const std::vector<std::vector<EntryPair>>& entries);
	
	size_t getRealPatternFragmentLen(const size_t iPatternFragment);
	size_t getRealTextFragmentLen(const size_t iTextFragment);
	
	
	
	void printResultToFile();
};


#endif /* TaskExecutor_hpp */

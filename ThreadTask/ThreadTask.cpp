//
//  ThreadTask.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#include "ThreadTask.hpp"



void prefixFunction(const char* s, int32_t* pi, const size_t len);
bool canBeOpened(const PathString& name);
inline bool fileFitsMask(const PathString& name, const regex& regexMask);

void searchWithPrefixFunc(const size_t patternLen, const char* s, int32_t* pi, const size_t len, std::set<int>& result);
void prefixFunction(const char* s, int32_t* pi, const size_t len);

size_t fileDoneSize = 0;
size_t numberOfFiles = 0;


enum BoolExpand {
	True, False, Undeterminacy
};

BoolExpand isRegular(const bfs::path& path) {
	try {
		
		if(bfs::is_regular_file(path)) {
			return True;
		}
		else {
			return False;
		}
	} catch (const bfs::filesystem_error& e) {
		return Undeterminacy;
	}
}

size_t TASKS_COUNTER = 0;

ThreadTask::ThreadTask(const bfs::path& path) : _path(path){
	++TASKS_COUNTER;
}

ThreadTask::ThreadTask(const ThreadTask& other) : _path(other._path) {
	++TASKS_COUNTER;
}

ThreadTask::~ThreadTask() {
	--TASKS_COUNTER;
}


void ThreadTask::doTask(const size_t cbMaxBufSize, char* buf, std::vector<ThreadTask> &newTasksFiles, std::vector<ThreadTask> &newTasksDirectories, const PathString& patternFileName, const regex& regexMask, std::vector<std::string>& result, std::vector<std::vector<std::set<int32_t>>>& entries) {

	_result = &result;
	_entries = &entries;
	
	const auto i1 = _path.string();
	const auto i2 = _path.wstring();
	const auto i3 = _path.u8string();
	const auto i4 = _path.u16string();

	_path.

	if(!canBeOpened(getStringOfPath(_path))) {
		return;
	}
	
	switch (isRegular(_path)) {
		case False:
			processDirectory(newTasksFiles, newTasksDirectories, regexMask);
			break;
		case True:
			processFile(cbMaxBufSize, buf, patternFileName);
			break;
		case Undeterminacy:
			;
	}
	
	return;
}


void ThreadTask::processDirectory(std::vector<ThreadTask> &newTasksFiles, std::vector<ThreadTask> &newTasksDirectories, const regex& regexMask) {
	
	const auto endIt = bfs::directory_iterator();
	
	for(auto it = bfs::directory_iterator(_path); it != endIt; ++it) {
		
//		printf("New task %s\n", it->path().c_str());
		
		const auto& path = it->path();
		const PathString& name = std::string(path.native().begin(), path.native().end());
		BoolExpand isFile = isRegular(path);
		
		if(isFile == Undeterminacy) {
			continue;
		}
		
		if(isFile == True) {
			if(!fileFitsMask(name, regexMask)) {
				continue;
			}
			newTasksFiles.push_back(path);
		}
		else {
			newTasksDirectories.push_back(path);
		}

	}
}

void ThreadTask::processFile(const size_t cbMaxBufSize, char* buf, const PathString& patternFileName) {
	
//	const PathString& textFileNativeName = _path.filename().native();

	searchInFile(cbMaxBufSize, buf, patternFileName);
}



void ThreadTask::searchInFile(const size_t cbMaxBufSize, char* buf, const PathString &patternFileName) {
	size_t patternLen;
	size_t textLen;
	
	patternLen  = bfs::file_size(bfs::path(patternFileName));
	textLen 	= bfs::file_size(_path);
	
	if(textLen > 1024 * 1024 * 10) {
		cout << getStringOfPath(_path) << " More Than 10MB size == " << textLen / (1024 * 1024) << " MB" << std::endl;
	}
	L"asd";
	if(textLen > 1024 * 1024 * 128) {
		cout << "! ! ! " << getStringOfPath(_path) << " Is TOO HUGE" << std::endl;
		return;
	}
	
	++numberOfFiles;
	fileDoneSize += textLen;
	
	if(patternLen > textLen) {
		return;
	}
	
	const size_t piArraySize = (cbMaxBufSize * 4) / 5;
	const size_t sArraySize	 = (cbMaxBufSize * 1) / 5;
	
	const size_t piArrayLen = piArraySize / 4;
	const size_t sArrayLen 	= sArraySize / 1;
	
	patternFragmentLen = ((sArrayLen - 1) * 1) / 7;
	textFragmentLen	= ((sArrayLen - 1) * 6) / 7;
	const size_t sharpSymbolLen 	= 1;
	
	int32_t* pi = (int32_t*)(buf);//new int32_t[piArrayLen];
	char* s = (buf + piArraySize);//new char[sArrayLen];
	
	
	
	
	numberOfFragmentsOfPattern = (patternLen + patternFragmentLen - 1) / patternFragmentLen;
	numberOfFragmentsOfTextWithImposition = 1 + (((long long)textLen - (long long)patternFragmentLen - 1) / (long long)(textFragmentLen - patternFragmentLen));
	
	_entries->resize(numberOfFragmentsOfPattern);
	for(size_t i = 0; i < numberOfFragmentsOfPattern; ++i) {
		(*_entries)[i].resize(numberOfFragmentsOfTextWithImposition);
		for(size_t j = 0; j < numberOfFragmentsOfTextWithImposition; ++j) {
			(*_entries)[i][j].clear();
		}
	}
	
//	entries = std::vector<std::vector<std::set<int32_t>>>(numberOfFragmentsOfPattern, std::vector<std::set<int32_t>>(numberOfFragmentsOfTextWithImposition));


	FILE* textFile = fopen( getString(getStringOfPath(_path)).c_str() , "rb");
	FILE* patternFile = fopen(getString(patternFileName).c_str(), "rb");
	
	
	for(size_t iPatternFragment = 0; iPatternFragment < numberOfFragmentsOfPattern; ++iPatternFragment) {
		
		for(size_t iTextFragment = 0; iTextFragment < numberOfFragmentsOfTextWithImposition; ++iTextFragment) {
			
			size_t realPatternFragmentLen;
			size_t realTextFramgentLen;
			
			if(iPatternFragment == numberOfFragmentsOfPattern - 1) {
				realPatternFragmentLen = patternLen - (numberOfFragmentsOfPattern - 1) * patternFragmentLen;
			}
			else {
				realPatternFragmentLen = patternFragmentLen;
			}
			
			if(iTextFragment == numberOfFragmentsOfTextWithImposition - 1) {
				realTextFramgentLen = textLen - (numberOfFragmentsOfTextWithImposition - 1) * (textFragmentLen - patternFragmentLen);
			}
			else {
				realTextFramgentLen = textFragmentLen;
			}
			
			downloadFragment(patternFile, iPatternFragment * patternFragmentLen, realPatternFragmentLen, s);
			
			s[realPatternFragmentLen] = '#';
			
			downloadFragment(textFile, iTextFragment * (textFragmentLen - patternFragmentLen), realTextFramgentLen, s + realPatternFragmentLen + 1);
			
			pi[0] = 0;
			
			searchWithPrefixFunc(realPatternFragmentLen, s, pi, realPatternFragmentLen + 1 + realTextFramgentLen, _entries[0][iPatternFragment][iTextFragment]);
		}
	}
	
//	delete [] pi;
//	delete [] s;
	
	fclose(textFile);
	fclose(patternFile);
	
	
	for(size_t iTextFramgent = 0; iTextFramgent < numberOfFragmentsOfTextWithImposition; ++iTextFramgent) {
		
		for(const auto& entryOfFirstFragmentOfPattern : _entries[0][0][iTextFramgent]) {
			
			const int startPositionOfPatternFragment = entryOfFirstFragmentOfPattern;
			
			size_t positionOfNextPatternFragment = startPositionOfPatternFragment + patternFragmentLen;
			
			size_t nextTextFragment;
			if(positionOfNextPatternFragment < textFragmentLen - patternFragmentLen) {
				nextTextFragment = iTextFramgent;
			}
			else {
				nextTextFragment = iTextFramgent + 1;
				positionOfNextPatternFragment -= (textFragmentLen - patternFragmentLen);
			}
			
			find(1, nextTextFragment, positionOfNextPatternFragment, startPositionOfPatternFragment + (textFragmentLen - patternFragmentLen) * iTextFramgent);
		}
		
	}
	
}



void ThreadTask::downloadFragment(FILE *file, size_t fromPosition, size_t size, char *buf) const {
	fseek(file, fromPosition, SEEK_SET);
	fread(buf, 1, size, file);
}


void ThreadTask::find(size_t crtPatternFragment, size_t iTextFragmentInWhichToFind, size_t iPositionWhereToFind, size_t startPatternPosition) {
	
	if(crtPatternFragment == numberOfFragmentsOfPattern) {

		_result->push_back("Found on position " + std::to_string(startPatternPosition) + "\n");
		return;
	}
	
	
	if(_entries[0][crtPatternFragment][iTextFragmentInWhichToFind].empty()){
		return;
	}
	
	{
//		middle = (double)(middle * counter + (double)entries[crtPatternFragment][iTextFragmentInWhichToFind].size()) / (double)(++counter);
		const auto entryOfFragmentOfPattern = _entries[0][crtPatternFragment][iTextFragmentInWhichToFind].find(iPositionWhereToFind);
		
		const int startPositionOfPatternFragment = *entryOfFragmentOfPattern;
		
		if(startPositionOfPatternFragment == iPositionWhereToFind){
			
			int positionOfNextPatternFragment = startPositionOfPatternFragment + patternFragmentLen;
			
			size_t nextTextFragment;
			if(positionOfNextPatternFragment < textFragmentLen - patternFragmentLen) {
				nextTextFragment = iTextFragmentInWhichToFind;
			}
			else {
				nextTextFragment = iTextFragmentInWhichToFind + 1;
				positionOfNextPatternFragment -= (textFragmentLen - patternFragmentLen);
			}
			
			find(crtPatternFragment + 1, nextTextFragment, positionOfNextPatternFragment, startPatternPosition);
		}
		
	}
	
	
}


inline bool fileFitsMask(const PathString& name, const regex& regexMask) {
	return std::regex_match(name, regexMask);
}

bool canBeOpened(const PathString& name) {

#if defined(_WIN32) || defined(_WIN64)
	FILE* file;
	
	const std::string nameString = getString(name);
	fopen_s(&file, nameString.c_str(), "r");
	bool res = (file != NULL);
	if (file != NULL) {
		fclose(file);
	}
	return res;
#else
	FILE* file = fopen( getCString(name) , "r");
	bool res = (file != NULL);
	if(file != NULL) {
		fclose(file);
	}
	return res;
#endif
}


void searchWithPrefixFunc(const size_t patternLen, const char* s, int32_t* pi, const size_t len, std::set<int32_t>& result) {
	
	prefixFunction(s, pi, len);
	
	size_t i = 2 * patternLen;
	while (i < len) {
		
		if(pi[i] == patternLen) {
			result.insert((int)(i - 2 * patternLen));
		}
		
		++i;
	}
}

void prefixFunction(const char* s, int32_t* pi, const size_t len){
	
	for(int i = 1; i < len; ++i) {
		int j = pi[i - 1];
		while(j > 0 && s[i] != s[j]) {
			j = pi[j - 1];
		}
		if(s[i] == s[j]) {
			++j;
		}
		pi[i] = j;
	}
	
	return;
}












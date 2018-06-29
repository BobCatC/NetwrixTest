//
//  TaskExecutor.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 28.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//

#include <string.h>
#include <iostream>

#include "TaskExecutor.hpp"


void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from);



TaskExecutor::TaskExecutor(const unsigned int threadID,
						   const std::string& outputFileName,
						   const size_t cbMaxBufSize,
						   const std::string& patternFileName,
						   const std::regex& regexMask) :
_threadID(threadID),
_outputFileName(outputFileName),
_cbMaxBufSize(cbMaxBufSize),
_patternFileName(patternFileName),
_regexMask(regexMask)
{
	openDefaultFiles();
	
	initBuffers();
}

void TaskExecutor::openDefaultFiles()
{
	_patternFile = fopen(_patternFileName.c_str(), "r");
	_outputFile = fopen(_outputFileName.c_str(), "w");
	
	if(_patternFile == nullptr)
	{
		throw std::string( "Couldn't open pattern file" );
	}
	if(_outputFile == nullptr)
	{
		throw std::string( "Couldn't open output file \"" + _outputFileName + "\"" );
	}
}

void TaskExecutor::initBuffers()
{	
	_buf = new char[_cbMaxBufSize];
	
	if(_buf == nullptr) {
		throw "Memory Allocation Error";
	}
	
	countDefaultMetrics();
	countPatternMetrics();
	
	_s = _buf;
	_pi = (int32_t*)(_buf + _sArraySize);
}

void TaskExecutor::countDefaultMetrics()
{
	_piArraySize = (_cbMaxBufSize * 4) / 5;
	_piArrayLen = _piArraySize / sizeof(int32_t);
	
	_sArraySize = (_cbMaxBufSize * 1) / 5;
	_sArrayLen = _sArraySize / sizeof(char);
}

void TaskExecutor::countPatternMetrics()
{
	_patternLen = bfs::file_size(bfs::path(_patternFileName));
	_patternFragmentLen = ((_sArrayLen - 1) * 1) / 7;
	if(_patternLen < _patternFragmentLen) {
		_patternFragmentLen = _patternLen;
	}
	_numberOfFragmentsOfPattern = (_patternLen + _patternFragmentLen - 1) / (_patternFragmentLen);
}

void TaskExecutor::countTextMetrics()
{
	_textLen = bfs::file_size(_path);
	_textFragmentLen = _sArraySize - 1 - _patternFragmentLen;
	_textFragmentWithImpositionLen = _textFragmentLen - _patternFragmentLen;
	_numberOfFragmentsOfTextWithImposition = 1 + (((long long)_textLen - (long long)_patternFragmentLen - 1) / (long long)(_textFragmentLen - _patternFragmentLen));
}



TaskExecutor::~TaskExecutor()
{
	if(_patternFile != nullptr) {
		fclose(_patternFile);
	}
	if(_outputFile != nullptr) {
		fclose(_outputFile);
	}
	
	
	if(_buf != nullptr) {
		delete [] _buf;
	}
}





//

void TaskExecutor::doTask(const ThreadTask& task, std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories)
{
	_path = task.getFileBfsPath();
	_textFileNativeName = task.getFileName();
	_textFilePath = task.getFilePath();
	
	_result.clear();
	
	try {
		
		if(bfs::is_directory(_path)) {
			processDirectory(newTasksFiles, newTasksDirectories);
		}
		else {
			processFile();
		}
		
	} catch (const bfs::filesystem_error& err) {
		
	} catch(const std::string& err) {
		
	} catch(const char* err) {
		
	}
	
	
	printResultToFile();
}


void TaskExecutor::processDirectory(std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories)
{
	const auto endIt = bfs::directory_iterator();
	
	++doneDirectories;
	for(auto it = bfs::directory_iterator(_path); it != endIt; ++it) {
		
		const bfs::path& newFilePath = it->path();
		
		destributeNewPath(newFilePath, newTasksFiles, newTasksDirectories);
		
	}
}

void TaskExecutor::destributeNewPath(const bfs::path &newFilePath, std::vector<std::string> &newTasksFiles, std::vector<std::string> &newTasksDirectories)
{
	
	if(bfs::is_directory(newFilePath)) {
		newTasksDirectories.push_back(newFilePath.string());
	}
	else {
		const std::string nativeName = newFilePath.filename().string();
		
		if (fileFitsMask(nativeName, _regexMask)) {
			
			newTasksFiles.push_back(newFilePath.string());
		}
	}
	
}

//	File

void TaskExecutor::processFile()
{
	countTextMetrics();
	
	++doneFiles;
	doneSize += _textLen;
	
	if(_patternLen > _textLen){
		return;
	}
	
	_textFile = fopen(_textFilePath.c_str(), "r");
	
	if(_textFile == nullptr) {
		throw ( "Couldn't open file \"" + _textFilePath + "\"");
	}
	
	std::vector<std::vector<EntryPair>> entries(_numberOfFragmentsOfTextWithImposition);
	
	int prefixFunctionDuration = 0;
	int siftDuration = 0;
	
	auto prefixStart = std::chrono::system_clock::now();
	findEntriesOfFirstFragment(entries);
	auto prefixEnd = std::chrono::system_clock::now();

	auto siftStart = std::chrono::system_clock::now();
	siftEntries(entries);
	auto siftEnd = std::chrono::system_clock::now();

	
	fillResult(entries);
	
	fclose(_textFile);
	
	prefixFunctionDuration = (int)std::chrono::duration_cast<std::chrono::microseconds>(prefixEnd - prefixStart).count();
	siftDuration = (int)std::chrono::duration_cast<std::chrono::microseconds>(siftEnd - siftStart).count();
	
	if((_textLen + _patternLen) / (1024 * 1024) > 2){
		std::cout << (_textLen + _patternLen) / (1024) << " KB\t";
		std::cout << "Prefix Function Duration : " << (prefixFunctionDuration / 1000) << std::endl;
		std::cout << "Sift Function Duration : " << (siftDuration / 1000) << std::endl;

		std::cout << std::endl;
//		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	
	allTimePrefix += prefixFunctionDuration;
}

void TaskExecutor::findEntriesOfFirstFragment(std::vector<std::vector<EntryPair> > &entries)
{
	size_t iPatternFragment = 0;
	
	for(size_t iTextFragment = 0; iTextFragment < _numberOfFragmentsOfTextWithImposition; ++iTextFragment) {
		
		size_t realPatternFragmentLen;
		size_t realTextFragmentLen;
		
		realPatternFragmentLen = getRealPatternFragmentLen(iPatternFragment);
		
		realTextFragmentLen = getRealTextFragmentLen(iTextFragment);
		if(iTextFragment != _numberOfFragmentsOfTextWithImposition - 1)
			--realTextFragmentLen;
		
		downloadFragment(_patternFile, iPatternFragment * _patternFragmentLen, realPatternFragmentLen, _s);
		
		_s[realPatternFragmentLen] = '#';
		downloadFragment(_textFile, iTextFragment * _textFragmentWithImpositionLen, realTextFragmentLen, _s + realPatternFragmentLen + 1);
		
		searchWithPrefixFunc(realPatternFragmentLen, iTextFragment, realPatternFragmentLen + 1 + realTextFragmentLen, entries[iTextFragment]);
		
	}
}

void TaskExecutor::searchWithPrefixFunc(const size_t realPatternFragmentLen, const size_t iTextFragment, const size_t len, std::vector<EntryPair>& result)
{
	_pi[0] = 0;
	prefixFunction(_s, _pi, len, 1);
	
//	for(size_t i = 0; i < len; ++i){
//		printf("%2i ", i);
//	}
//	std::cout << std::endl;
//	for(size_t i = 0; i < len; ++i){
//		printf("%2c ", _s[i]);
//	}
//	std::cout << std::endl;
//	for(size_t i = 0; i < len; ++i){
//		printf("%2i ", _pi[i]);
//	}
//	std::cout << std::endl << std::endl;
//
	
	size_t i = 2 * realPatternFragmentLen;
	while (i < len) {
		
		if(_pi[i] >= realPatternFragmentLen) {
		
			size_t j = i;
			while(_pi[j] > realPatternFragmentLen) {
				j = _pi[j] - 1;
			}
			_pi[i] = _pi[j];
			
			if(_pi[j] == realPatternFragmentLen) {
			
				CrtFragmentEntry position = (CrtFragmentEntry)(i - 2 * realPatternFragmentLen);
				FirstFragmentEntry absolutePosition = (FirstFragmentEntry)(position + (iTextFragment * _textFragmentWithImpositionLen));
				EntryPair pair(absolutePosition, position);
			
				if(firstPatternFragmentCanBeOnPosition(position, iTextFragment)) {
					result.push_back(pair);
				}
			}
		}
		
		++i;
	}
}


bool TaskExecutor::firstPatternFragmentCanBeOnPosition(size_t position, size_t iTextFragment)
{
	size_t absolutePosition = position + (iTextFragment * _textFragmentWithImpositionLen);
	const size_t maxSizeForPosition = _textLen - absolutePosition;
	
	return ( _patternLen <= maxSizeForPosition );
}

void TaskExecutor::siftEntries(std::vector<std::vector<EntryPair> > &entries)
{
	
	for(size_t iPatternFragment = 1; iPatternFragment < _numberOfFragmentsOfPattern; ++iPatternFragment) {
		
		size_t realPatternFragmentLen;
		size_t realCrtTextFragmentLen, realNextTextFragmentLen;
		
		realPatternFragmentLen = getRealPatternFragmentLen(iPatternFragment);
		realNextTextFragmentLen = getRealTextFragmentLen(0);
		
		char* crtPatternFragment = _buf;
		downloadFragment(_patternFile, iPatternFragment * _patternFragmentLen, realPatternFragmentLen, crtPatternFragment);
		crtPatternFragment[realPatternFragmentLen] = 0;
		
		char* crtTextFragment, *nextTextFragment;
		crtTextFragment = _buf + _patternFragmentLen + 1;
		nextTextFragment = _buf + _patternFragmentLen + 2 + _textFragmentLen;
		
		
		downloadFragment(_textFile, 0, realNextTextFragmentLen, nextTextFragment);
		nextTextFragment[realNextTextFragmentLen] = 0;
		
		
		for(int iTextFragment = 0; iTextFragment < _numberOfFragmentsOfTextWithImposition; ++iTextFragment) {
			
			std::swap(realCrtTextFragmentLen, realNextTextFragmentLen);
			std::swap(crtTextFragment, nextTextFragment);
			
			realNextTextFragmentLen = getRealTextFragmentLen(iTextFragment + 1);
			
			downloadFragment(_textFile, (iTextFragment + 1) * (_textFragmentLen - _patternFragmentLen), realNextTextFragmentLen, nextTextFragment);
			nextTextFragment[realNextTextFragmentLen] = 0;
			
			std::vector<EntryPair> filteredEntries;
			filteredEntries.reserve(entries[iTextFragment].size());
			
			
			for(const auto& pair : entries[iTextFragment]) {
				
				const unsigned long long startPositionOfPatternFragment = pair.second;
				
				size_t positionOfNextPatternFragment = startPositionOfPatternFragment + _patternFragmentLen;
				
				char* textFragment;
				size_t textFragmentLen;
				
				if(positionOfNextPatternFragment < _textFragmentLen - _patternFragmentLen) {
					textFragment = crtTextFragment;
					textFragmentLen = realCrtTextFragmentLen;
				}
				else {
					textFragment = nextTextFragment;
					textFragmentLen = realNextTextFragmentLen;
					positionOfNextPatternFragment -= (_textFragmentLen - _patternFragmentLen);
				}
				
				if(patternFragmentFitsTextFragment(crtPatternFragment, realPatternFragmentLen, textFragment, textFragmentLen, positionOfNextPatternFragment))
				{
					filteredEntries.push_back(pair);
				}
				
			}
			
			entries[iTextFragment] = filteredEntries;
			
			
		}
		
		if(iPatternFragment != _numberOfFragmentsOfPattern - 1) {
			increasePositions(entries);
		}
		
	}
	
}

size_t TaskExecutor::getRealPatternFragmentLen(const size_t iPatternFragment)
{
	size_t realPatternFragmentLen;
	if(iPatternFragment == _numberOfFragmentsOfPattern - 1) {
		realPatternFragmentLen = _patternLen - (_numberOfFragmentsOfPattern - 1) * _patternFragmentLen;
	}
	else {
		realPatternFragmentLen = _patternFragmentLen;
	}
	return realPatternFragmentLen;
	
}
size_t TaskExecutor::getRealTextFragmentLen(const size_t iTextFragment)
{
	size_t realTextFragmentLen;
	if(iTextFragment == _numberOfFragmentsOfTextWithImposition - 1) {
		realTextFragmentLen = _textLen - (_numberOfFragmentsOfTextWithImposition - 1) * _textFragmentWithImpositionLen;
	}
	else {
		realTextFragmentLen = _textFragmentLen;
	}
	return realTextFragmentLen;
}


bool TaskExecutor::patternFragmentFitsTextFragment(const char* patternFragment, size_t realPatternFragmentLen, const char* textFragment, size_t realTextFragmentLen, size_t positionInTextFragment)
{
	if(realPatternFragmentLen > realTextFragmentLen - positionInTextFragment) {
		return false;
	}
	
	return strncmp(patternFragment, textFragment + positionInTextFragment, realPatternFragmentLen) == 0;
}

void TaskExecutor::increasePositions(std::vector<std::vector<EntryPair>> &entries)
{
	std::vector<EntryPair> filteredEntriesCrt, filteredEntriesNext;
	
	for(size_t iTextFragment = 0; iTextFragment < _numberOfFragmentsOfTextWithImposition; ++iTextFragment)
	{
		
		filteredEntriesCrt = filteredEntriesNext;
		filteredEntriesNext.clear();
		
		for(auto pair : entries[iTextFragment])
		{
			const unsigned long long startPositionOfPatternFragment = pair.second;
			
			size_t positionOfNextPatternFragment = startPositionOfPatternFragment + _patternFragmentLen;

			size_t textFragment;
			std::vector<EntryPair>* filteredEntriesPointer = nullptr;
			
			if(positionOfNextPatternFragment < _textFragmentWithImpositionLen) {
				textFragment = iTextFragment;
				filteredEntriesPointer = &filteredEntriesCrt;
			}
			else {
				textFragment = iTextFragment + 1;
				positionOfNextPatternFragment -= _textFragmentWithImpositionLen;
				filteredEntriesPointer = &filteredEntriesNext;
			}
			
			if(textFragment >= _numberOfFragmentsOfTextWithImposition) {
				
			}
			else {
				EntryPair newPair(pair);
				newPair.second = (CrtFragmentEntry)positionOfNextPatternFragment;
				
				filteredEntriesPointer->push_back(newPair);
			}
			
		}
		
		entries[iTextFragment] = filteredEntriesCrt;
		
	}
	
}

void TaskExecutor::fillResult(const std::vector<std::vector<EntryPair>>& entries)
{
	
	for(size_t iTextFragment = 0; iTextFragment < _numberOfFragmentsOfTextWithImposition; ++iTextFragment)
	{
		for(const auto& pair : entries[iTextFragment])
		{
			const FirstFragmentEntry position = pair.first;
			_result.push_back((int)position);
		}

	}
}

//	Prefix Function


void prefixFunction(const char* s, int32_t* pi, const size_t len, const int from)
{
	for(int i = from; i < len; ++i) {
		int j = pi[i - 1];
		while(j > 0 && s[i] != s[j]) {
			j = pi[j - 1];
		}
		if(s[i] == s[j]) {
			++j;
		}
		pi[i] = j;
	}
}

void TaskExecutor::printResultToFile()
{
	if(!_result.empty()) {
		
		const size_t cbMaxResultBufSize = _cbMaxBufSize - 128;
		assert(_cbMaxBufSize > 128);
		
		size_t i = 0;
		size_t size = _result.size();
		size_t cbResultBufSize = sprintf(_s, "*** In File %s\n", _textFilePath.c_str());
		
		while(i < size)
		{
			
			while(i < size && cbResultBufSize < cbMaxResultBufSize)
			{
				cbResultBufSize += sprintf(_s + cbResultBufSize, "\tposition : %i\n", _result[i]);
				++i;
			}
			
			fwrite(_s, 1, cbResultBufSize, _outputFile);
			cbResultBufSize = 0;
		}
		
		
	}
	
	
}













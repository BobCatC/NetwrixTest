//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//


#include "Request/Request.hpp"
#include "FileSystem/FileSystem.hpp"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( RequestTest )

BOOST_AUTO_TEST_CASE( NoMask )
{
	Request req;
	const char* argv[] = {"runtime_dir", "-p", "/usr", "-i", "string.txt", "-o", "output.txt"};
	int argc = 7;
	
	req.parseArguments(argc, argv);
	
	BOOST_REQUIRE_EQUAL(req.mask, std::string("*.*"));
}

BOOST_AUTO_TEST_CASE( RussianSymbolsParsing )
{
	Request req;
	const char* argv[] = {"рантайм", "-p", "/окошки", "-i", "паттерн.txt", "-o", "вывод.txt"};
	int argc = 7;
	
	BOOST_REQUIRE_NO_THROW(req.parseArguments(argc, argv));
	
	BOOST_REQUIRE_EQUAL(req.startDirectory, "/окошки");
	BOOST_REQUIRE_EQUAL(req.patternFileName, "паттерн.txt");
	BOOST_REQUIRE_EQUAL(req.outputFileName, "вывод.txt");
	BOOST_REQUIRE_EQUAL(req.mask, "*.*");
}

BOOST_AUTO_TEST_CASE( WithoutOutput )
{
	Request req;
	const char* argv[] = {"рантайм", "-p", "/окошки", "-i", "паттерн.txt"};
	int argc = 5;
	
	BOOST_REQUIRE_THROW(req.parseArguments(argc, argv), std::string);
}

BOOST_AUTO_TEST_CASE( WithoutPattern )
{
	Request req;
	const char* argv[] = {"рантайм", "-p", "/окошки", "-o", "вывод.txt"};
	int argc = 5;
	
	BOOST_REQUIRE_THROW(req.parseArguments(argc, argv), std::string);
}

BOOST_AUTO_TEST_CASE( WithoutStartDir )
{
	Request req;
	const char* argv[] = {"рантайм", "-o", "вывод.txt", "-i", "вывод.txt"};
	int argc = 5;
	
	BOOST_REQUIRE_THROW(req.parseArguments(argc, argv), std::string);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( FileSystemTest )

BOOST_AUTO_TEST_CASE( ExistFileTest )
{
	const std::string fileName("newFile12341234.txt");
	FILE* file = fopen(fileName.c_str(), "w");
	BOOST_CHECK(file != NULL);
	
	bool res = fileWithNameExists(fileName);
	
	fclose(file);
	remove(fileName.c_str());

	BOOST_CHECK(res == true);
}

BOOST_AUTO_TEST_CASE( NotExistFileTest )
{
	// hope it doesn't exist)
	const std::string fileName("newFile123asndjkanskndkljn41234.txat");
	
	bool res = fileWithNameExists(fileName);
	
	BOOST_CHECK(res == false);
}


BOOST_AUTO_TEST_CASE( FindFreeFileNameTest )
{
	const std::string testExistsFileName("TestNAME123124nasdnnljn");
	
	FILE* file = fopen(testExistsFileName.c_str(), "w+");
	BOOST_CHECK(file != NULL);
	fclose(file);
	
	const std::string freeName = findFreeName(testExistsFileName, "");
	
	BOOST_CHECK(freeName != "");
	BOOST_CHECK(freeName != testExistsFileName);
	
	remove(testExistsFileName.c_str());
}

BOOST_AUTO_TEST_CASE( MaskTesting )
{
	std::string name;
	std::regex mask;
	name = "abc.def";
	mask = ".*[.].*";
	BOOST_CHECK(fileFitsMask(name, mask));
	
	mask = ".*";
	BOOST_CHECK(fileFitsMask(name, mask));
	
	mask = ".*.*";
	BOOST_CHECK(fileFitsMask(name, mask));
	
	mask = ".[.].*";
	BOOST_CHECK(fileFitsMask(name, mask) == false);
	
	name = "abc";
	
	mask = ".[.].*";
	BOOST_CHECK(fileFitsMask(name, mask) == false);

	mask = ".*";
	BOOST_CHECK(fileFitsMask(name, mask));
	
	mask = ".*[.].*";
	BOOST_CHECK(fileFitsMask(name, mask) == false);
	
	name = "a.b";
	mask = ".[.].";
	BOOST_CHECK(fileFitsMask(name, mask));
	
	mask = "...";
	BOOST_CHECK(fileFitsMask(name, mask));
}

BOOST_AUTO_TEST_SUITE_END()











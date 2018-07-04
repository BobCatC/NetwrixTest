//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//


#include "Request/Request.hpp"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ArgumentsParsingTest)

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



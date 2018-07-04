//
//  main.cpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 23.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved(no).
//



class Person
{
	
public:
	Person(const char* name)
	{
	}
private:
	const char* name;
};

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Testing1)

BOOST_AUTO_TEST_CASE( my_test )
{
	Person test_object( "qwerty" );
	
}


BOOST_AUTO_TEST_SUITE_END()



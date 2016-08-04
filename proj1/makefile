#makefile for assembler project

default:	main.cpp
	g++ -Werror -mtune=generic -O0 -std=c++11 -omain main.cpp
	chmod 700 main

test:		test.cpp
	g++ -Werror -mtune=generic -O0 -std=c++11 -otest test.cpp
	chmod 700 test


debug	:	main.cpp
	g++ -Werror -mtune=generic -O0 -DDEBUG -std=c++11 -odebug main.cpp
	chmod 700 debug

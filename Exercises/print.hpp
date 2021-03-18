///
/// Utilitaires pour afficher les ctor/dtor d'une classe donnée.
///

#pragma once


#include <cstddef>
#include <cstdint>
#include <new>

#include <cstdio>
#include <iostream>
#include <string>

#include <list>

using namespace std;


void printDefCtor ( const char* c )
{
	printf("%s::%s()" "\n", c, c);
}

void printCpyCtor ( const char* c )
{
	printf("%s::%s(const %s&)" "\n", c, c, c);
}

void printMovCtor ( const char* c )
{
	printf("%s::%s(%s&&)" "\n", c, c, c);
}

void printIntParamCtor ( const char* c )
{
	printf("%s::%s(int)" "\n", c, c);
}

void printStrParamCtor ( const char* c )
{
	printf("%s::%s(const string&)" "\n", c, c);
}

void printDtor ( const char* c )
{
	printf("%s::~%s()" "\n", c, c);
}

void printCpyAsgn ( const char* c )
{
	printf("%s::operator=(const %s&)" "\n", c, c);
}

void printMovAsgn ( const char* c )
{
	printf("%s::operator=(%s&&)" "\n", c, c);
}

/*
 *  converter.h
 *  AlephOne-OSX10.4
 *
 *  Created by みちあき on 08/06/24.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 *  Modified by Logue on 19/04/09
 */
#ifndef _CSERIES_CONVERTER_
#define _CSERIES_CONVERTER_

#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <string>
#include <boost/locale/encoding.hpp>

char* sjis2utf8(const char* str, size_t len);
char* utf82sjis(const char* str, size_t len);

unsigned short* sjis2utf16(const char* str, size_t len);
unsigned short* utf82utf16(const char* str, size_t len);
char* utf162utf8(const unsigned short* str, size_t len);

typedef unsigned short uint16;
uint16 sjisChar(char* in, int* step);
int unicodeChar( const char* input, uint16* ret);

bool isJChar(unsigned char text);
bool is2ndJChar(unsigned char text);

std::string utf82sjis(const std::string& input);
std::string sjis2utf8(const std::string& input);
std::string utf82utf16(const std::string& input);
std::string utf162utf8(const std::string& input);

#define _SJIS(str) utf82sjis(str).c_str()
#define _UTF8(str) sjis2utf8(str).c_str()
#define _UTF16(str) utf82utf16(str).c_str()
#endif
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

#include <vector>
#include <string>
#include <iconv.h>
#include <boost/locale/encoding.hpp>
#include "SDL_ttf.h"

#define MAC_LINE_END 13
#define _SJIS(str) utf82sjis(str).c_str()
#define _UTF8(str) sjis2utf8(str).c_str()

inline int utf8_len(const char *t)
{
    unsigned char c = *t;
    if (c <= 0xc2)
        return 1;
    if (c <= 0xdf)
        return 2;
    if (c <= 0xef)
        return 3;
    return 4;
}

// Detect 2-byte char. (for Shift_JIS)
inline bool isJChar(unsigned char text)
{
    return (((text >= 0x81) && (text <= 0x9f)) || ((text >= 0xe0) && (text <= 0xfc))) ? true : false;
}

// Detect 2nd charactor of 2-byte char. (for Shift_JIS)
inline bool is2ndJChar(unsigned char text)
{
    return (((0x7F != text) && (0x40 <= text)) || ((text >= 0xe0) && (text <= 0xfc))) ? true : false;
}

class utf8_iter
{
    std::string s;
    size_t i;

public:
    utf8_iter(const std::string &s) : s(s), i(0) {}
    std::string utf8()
    {
        return s.substr(i, utf8_len(&s[i]));
    }
    char32_t code()
    {
        switch (utf8_len(&s[i]))
        {
        case 2:
            return ((s[i] & 0x1f) << 6) | s[i + 1] & 0x3f;
        case 3:
            return ((s[i] & 0xf) << 12) |
                   ((s[i + 1] & 0x3f) << 6) |
                   ((s[i + 2] & 0x3f));
        case 4:
            return ((s[i] & 0x7) << 18) |
                   ((s[i + 1] & 0x3f) << 12) |
                   ((s[i + 2] & 0x3f) << 6) |
                   ((s[i + 3] & 0x3f));
        default:
            return s[i];
        }
    }
    bool begin() { return i == 0; }
    bool end() { return i >= s.size(); }
    utf8_iter &operator++()
    {
        const char *c = &s[i];
        i += utf8_len(c);
        return *this;
    }
    utf8_iter &operator--()
    {
        while (i > 0 &&
               (unsigned char)s[i - 1] >= 0x80 &&
               (unsigned char)s[i - 1] < 0xc0)
        {
            --i;
        }
        --i;
        return *this;
    }
};

std::string utf82sjis(const std::string &input);
std::string sjis2utf8(const std::string &input);
std::string utf82utf16(const std::string &input);
std::string utf162utf8(const std::string &input);

std::string sjis2utf8(const char *str, size_t len);

typedef unsigned short uint16;
uint16 sjisChar(char *in, int *step);
void sjisChar(const char *in, int *step, char *dst);
std::vector<std::string> line_wrap(TTF_Font *t, const std::string &str, int size);
const char *line_wrap_term(TTF_Font *t, const char *begin, const char *end, int size);

#endif

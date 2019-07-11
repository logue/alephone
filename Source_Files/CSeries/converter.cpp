/*
 *  converter.cpp
 *
 *  Created by みちあき on 08/06/24.
 *  Modified by Logue 2019/07/10
 */
#include <iconv.h>
#include <boost/locale/encoding.hpp>
#include "converter.h"

// Shift_JIS to UTF8
std::string sjis2utf8(const std::string &input)
{
    return !input.empty() ? boost::locale::conv::to_utf<char>(input, "Shift_JIS") : "";
}

// Shift_JIS to UTF8 (for conpatibility)
std::string sjis2utf8(const char *str, size_t len)
{
    std::string input = str;
    return !input.empty() ? boost::locale::conv::to_utf<char>(input, "Shift_JIS") : "";
}

// UTF8 to Shitf_JIS
std::string utf82sjis(const std::string &input)
{
    return !input.empty() ? boost::locale::conv::from_utf(input, "Shift_JIS") : "";
}

// UTF8 to Unicode
std::string utf82utf16(const std::string &input)
{
    return !input.empty() ? boost::locale::conv::from_utf<char>(input, "UCS-2LE") : "";
}

// Unicode to UTF8
std::string utf162utf8(const std::string &input)
{
    return !input.empty() ? boost::locale::conv::to_utf<char>(input, "UCS-2LE") : "";
}

// Convert from Shift_JIS to Unicode
void sjisChar(const char *in, int *step, char *dst)
{
    size_t len;
    if (*in == MAC_LINE_END)
    {
        if (step)
        {
            *step += 1;
        }
        *dst = MAC_LINE_END;
        return;
    }
    if (step)
    {
        if (isJChar(*in))
        {
            *step += 2;
            len = 2;
        }
        else
        {
            *step += 1;
            len = 1;
        }
    }
    /*
    char *strp = (char *)in;
    char *retp = dst;
    size_t sz = 4;
    iconv_t i = iconv_open("UTF-8", "SHIFT-JIS");
    if (iconv(i, &strp, &len, &retp, &sz) == -1)
    {
        iconv_t j = iconv_open("UTF-8", "MACROMAN");
        strp = (char *)in;
        retp = dst;
        sz = 4;
        iconv(j, &strp, &len, &retp, &sz);
        iconv_close(j);
    }
    iconv_close(i);
	*/
    // UTF-8 -> Shift_JIS
    std::string output = boost::locale::conv::from_utf<char>(in, "Shift_JIS");
    dst = (char *)output.c_str();
}

uint16 sjisChar(char *in, int *step)
{
    size_t len;
    if (*in == MAC_LINE_END)
    {
        if (step)
            *step += 1;
        return MAC_LINE_END;
    }
    if (step)
    {
        if (isJChar((unsigned char)*in))
        {
            *step += 2;
            len = 2;
        }
        else
        {
            *step += 1;
            len = 1;
        }
    }
    uint16 text[2];

    memset(text, 0, 4);
    char *strp = in;
    char *retp = (char *)text;
    size_t sz = 4;
    iconv_t i = iconv_open("UCS-2LE", "SHIFT-JIS");
    iconv_t j = iconv_open("UCS-2LE", "MACROMAN");
    if (iconv(i, &strp, &len, &retp, &sz) == -1)
    {
        strp = in;
        retp = (char *)text;
        sz = 4;
        iconv(j, &strp, &len, &retp, &sz);
    }
    iconv_close(i);
    iconv_close(j);
    /*
    // Unicode -> UTF-8
    std::string utf = boost::locale::conv::to_utf(in, "UCS-2LE");
    // UTF-8 -> Shift_JIS
    std::string sjis = boost::locale::conv::from_utf<char>(utf, "Shift_JIS");
    text = (char *)sjis.c_str();
*/
    return text[0];
}

// str is UTF-8
std::vector<std::string> line_wrap(TTF_Font *t, const std::string &str, int size)
{
    std::vector<std::string> ret;
    std::string now;
    utf8_iter it(str);
    int w = 0;
    while (!it.end())
    {
        char32_t c = it.code();
        int w2;
        std::string tmp = it.utf8();
        TTF_SizeUTF8(t, tmp.c_str(), &w2, nullptr);
        if (c >= 0x3040 && c <= 0x9fef ||
            c >= 0x20000 && c <= 0x2ebe0)
        {
            // fetch next letter
            int w2;
            if (w + w2 < size)
            {
                now += tmp;
                w += w2;
            }
            else
            {
                ret.push_back(now);
                now = tmp;
                w = 0;
            }
        }
        else if (isspace(c))
        {
            // don't care overflow
            now += it.utf8();
        }
        else
        {
            // fetch next word
            std::string tmp;
            while (!it.end())
            {
                char32_t c = it.code();
                if (isspace(c) ||
                    c >= 0x3040 && c <= 0x9fef ||
                    c >= 0x20000 && c <= 0x2ebe0)
                {
                    break;
                }
                else
                {
                    tmp += it.utf8();
                }
            }
            TTF_SizeUTF8(t, tmp.c_str(), &w2, 0);
            if (w > size)
            {
                ret.push_back(now);
                now = tmp;
            }
            else
            {
                now += tmp;
            }
        }
    }
    if (!now.empty())
    {
        ret.push_back(now);
    }
    return ret;
}
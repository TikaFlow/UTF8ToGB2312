/*
 * =============================================================================
 *
 *       Filename:  UTF8ToGB2312.cpp
 *
 *    Description:  Converts UTF-8 string to GB2312 string in Arduino(GB2312 library is included).
 *
 *        Version:  1.1.52
 *        Created:  2023-09-23 14:17:34
 *
 *         Author:  Tika Flow
 *
 * =============================================================================
 */

/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/
#include "UTF8ToGB2312.h"

/**
 * @brief convert Unicode character to GB2312 character
 *
 * @param ub Unibytes which contains Unicode and bytes count of the UTF-8 character (1 ~ 6)
 *
 * @returns GB2312 character
 */
String U2GB::toGB2312(Unibytes ub)
{
    char t[3] = {0};
    String res = "";
    if (ub.bytes == 1)
    {
        t[0] = 0xff & ub.unicode;
    }
    else
    {
        ucs4_t wc = ub.unicode;
        const Summary16 *summary = NULL;
        if (wc >= 0x0000 && wc < 0x0460)
        {
            summary = &gb2312_uni2indx_page00[(wc >> 4) - 0x000];
        }
        else if (wc >= 0x2000 && wc < 0x2650)
        {
            summary = &gb2312_uni2indx_page20[(wc >> 4) - 0x200];
        }
        else if (wc >= 0x3000 && wc < 0x3230)
        {
            summary = &gb2312_uni2indx_page30[(wc >> 4) - 0x300];
        }
        else if (wc >= 0x4e00 && wc < 0x9cf0)
        {
            summary = &gb2312_uni2indx_page4e[(wc >> 4) - 0x4e0];
        }
        else if (wc >= 0x9e00 && wc < 0x9fb0)
        {
            summary = &gb2312_uni2indx_page9e[(wc >> 4) - 0x9e0];
        }
        else if (wc >= 0xff00 && wc < 0xfff0)
        {
            summary = &gb2312_uni2indx_pageff[(wc >> 4) - 0xff0];
        }
        if (summary)
        {
            unsigned short used = summary->used;
            unsigned int i = wc & 0x0f;
            if (used & ((unsigned short)1 << i))
            {
                unsigned short c;
                /* Keep in `used' only the bits 0..i-1. */
                used &= ((unsigned short)1 << i) - 1;
                /* Add `summary->indx' and the number of bits set in `used'. */
                used = (used & 0x5555) + ((used & 0xaaaa) >> 1);
                used = (used & 0x3333) + ((used & 0xcccc) >> 2);
                used = (used & 0x0f0f) + ((used & 0xf0f0) >> 4);
                used = (used & 0x00ff) + (used >> 8);
                c = gb2312_2charset[summary->indx + used];
                t[0] = (c >> 8) | 0x80;
                t[1] = (c & 0xff) | 0x80;
            }
        }
    }
    res += t;
    return res;
};

/**
 * @brief convert UTF-8 character to Unicode character
 *
 * @param str UTF-8 character which to be converted
 *
 * @returns Unibytes which contains Unicode and bytes count of the UTF-8 character (1 ~ 6)
 */
Unibytes U2GB::getUnicodeChar(char *str)
{
    unsigned char *p = (unsigned char *)str;
    unsigned int e = 0;
    Unibytes ub;
    if (*p >= 0xfc)
    {
        /* 6:<1111110a 10bbbbbb 10cccccc 10dddddd 10eeeeee 10ffffff> */
        e = (p[0] & 0x01) << 30;  // e=0a000000 00000000 00000000 00000000
        e |= (p[1] & 0x3f) << 24; // e=0abbbbbb 00000000 00000000 00000000
        e |= (p[2] & 0x3f) << 18; // e=0abbbbbb cccccc00 00000000 00000000
        e |= (p[3] & 0x3f) << 12; // e=0abbbbbb ccccccdd dddd0000 00000000
        e |= (p[4] & 0x3f) << 6;  // e=0abbbbbb ccccccdd ddddeeee ee000000
        e |= (p[5] & 0x3f);       // e=0abbbbbb ccccccdd ddddeeee eeffffff
        ub.bytes = 6;
    }
    else if (*p >= 0xf8)
    {
        /* 5:<111110aa 10bbbbbb 10cccccc 10dddddd 10eeeeee> */
        e = (p[0] & 0x03) << 24;  // e=000000aa 00000000 00000000 00000000
        e |= (p[1] & 0x3f) << 18; // e=000000aa bbbbbb00 00000000 00000000
        e |= (p[2] & 0x3f) << 12; // e=000000aa bbbbbbcc cccc0000 00000000
        e |= (p[3] & 0x3f) << 6;  // e=000000aa bbbbbbcc ccccdddd dd000000
        e |= (p[4] & 0x3f);       // e=000000aa bbbbbbcc ccccdddd ddeeeeee
        ub.bytes = 5;
    }
    else if (*p >= 0xf0)
    {
        /* 4:<11110aaa 10bbbbbb 10cccccc 10dddddd> */
        e = (p[0] & 0x07) << 18;  // e=00000000 000aaa00 00000000 00000000
        e |= (p[1] & 0x3f) << 12; // e=00000000 000aaabb bbbb0000 00000000
        e |= (p[2] & 0x3f) << 6;  // e=00000000 000aaabb bbbbcccc cc000000
        e |= (p[3] & 0x3f);       // e=00000000 000aaabb bbbbcccc ccdddddd
        ub.bytes = 4;
    }
    else if (*p >= 0xe0)
    {
        /* 3:<1110aaaa 10bbbbbb 10cccccc> */
        e = (p[0] & 0x0f) << 12; // e=00000000 00000000 aaaa0000 00000000
        e |= (p[1] & 0x3f) << 6; // e=00000000 00000000 aaaabbbb bb000000
        e |= (p[2] & 0x3f);      // e=00000000 00000000 aaaabbbb bbcccccc
        ub.bytes = 3;
    }
    else if (*p >= 0xc0)
    {
        /* 2:<110aaaaa 10bbbbbb> */
        e = (p[0] & 0x1f) << 6; // e=00000000 00000000 00000aaa aa000000
        e |= (p[1] & 0x3f);     // e=00000000 00000000 00000aaa aabbbbbb
        ub.bytes = 2;
    }
    else
    {
        e = p[0];
        ub.bytes = 1;
    }
    ub.unicode = e;

    return ub;
}

/**
 * @brief convert UTF-8 string to GB2312 string
 *
 * @param utf8_str UTF-8 string which to be converted
 *
 * @returns GB2312 string
 */
String U2GB::get(String utf8_str)
{
    Unibytes ub;
    String res = "";
    int i = 0;

    while (utf8_str[i])
    {
        ub = getUnicodeChar(&utf8_str[i]);

        res += toGB2312(ub);
        i += ub.bytes;
    }

    return res;
}

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
 * @param ub Unibytes which contains Unicode and bytes count of the UTF-8 character (1 ~ 6)
 * @returns GB2312 character
 */
String U2GB::toGB2312(Unibytes ub) {
    char t[3] = {0};
    String res = "";
    bool is_gb2312_missing = false;

    if (ub.bytes == 1) {
        // 单字节字符直接转换
        t[0] = 0xff & ub.unicode;
    } else {
        ucs4_t wc = ub.unicode;
        const Summary16 *summary = NULL;
        
        // 根据Unicode范围查找对应的索引页
        if (wc >= 0x0000 && wc < 0x0460) {
            summary = &gb2312_uni2indx_page00[(wc >> 4) - 0x000];
        } else if (wc >= 0x2000 && wc < 0x2650) {
            summary = &gb2312_uni2indx_page20[(wc >> 4) - 0x200];
        } else if (wc >= 0x3000 && wc < 0x3230) {
            summary = &gb2312_uni2indx_page30[(wc >> 4) - 0x300];
        } else if (wc >= 0x4e00 && wc < 0x9cf0) {
            summary = &gb2312_uni2indx_page4e[(wc >> 4) - 0x4e0];
        } else if (wc >= 0x9e00 && wc < 0x9fb0) {
            summary = &gb2312_uni2indx_page9e[(wc >> 4) - 0x9e0];
        } else if (wc >= 0xff00 && wc < 0xfff0) {
            summary = &gb2312_uni2indx_pageff[(wc >> 4) - 0xff0];
        } else {
            // Unicode不在GB2312支持范围内
            is_gb2312_missing = true;
        }

        if (summary) {
            unsigned short used = summary->used;
            unsigned int i = wc & 0x0f;
            
            // 检查该字符是否在当前页中有定义
            if (used & ((unsigned short) 1 << i)) {
                unsigned short c;
                /* 计算在字符集中的索引 */
                used &= ((unsigned short) 1 << i) - 1;
                used = (used & 0x5555) + ((used & 0xaaaa) >> 1);
                used = (used & 0x3333) + ((used & 0xcccc) >> 2);
                used = (used & 0x0f0f) + ((used & 0xf0f0) >> 4);
                used = (used & 0x00ff) + (used >> 8);
                
                // 从PROGMEM中读取GB2312编码
                c = pgm_read_word(&gb2312_2charset[summary->indx + used]);
                t[0] = (c >> 8) | 0x80;
                t[1] = (c & 0xff) | 0x80;
            } else {
                // 当前页中未找到该字符
                is_gb2312_missing = true;
            }
        } else if (!is_gb2312_missing) {
            // 未找到对应的索引页
            is_gb2312_missing = true;
        }
    }

    // 根据错误类型使用不同的替代字符
    if (is_gb2312_missing) {
        // GB2312未收录字符，使用'?'
        t[0] = 0x3F;
        t[1] = 0x3F;
    }

    res += t;
    return res;
};

/**
 * @brief convert UTF-8 character to Unicode character
 * @param str UTF-8 character which to be converted
 * @returns Unibytes which contains Unicode and bytes count of the UTF-8 character (1 ~ 6)
 */
Unibytes U2GB::getUnicodeChar(char *str) {
    unsigned char *p = (unsigned char *) str;
    unsigned int e = 0;
    Unibytes ub = {0, 0}; // 初始化bytes为0表示无效

    // 检查首字节是否为非法的7个或8个前置1
    if (*p >= 0xfe) {
        // 0xFE和0xFF是非法的UTF-8首字节
    } else if (*p >= 0xfc) {
        /* 6字节UTF-8: 1111110a 10bbbbbb 10cccccc 10dddddd 10eeeeee 10ffffff */
        if (p[1] < 0x80 || p[1] >= 0xc0 || p[2] < 0x80 || p[2] >= 0xc0 ||
            p[3] < 0x80 || p[3] >= 0xc0 || p[4] < 0x80 || p[4] >= 0xc0 ||
            p[5] < 0x80 || p[5] >= 0xc0) {
            // 后续字节必须为10xxxxxx格式
        } else {
            e = (p[0] & 0x01) << 30;
            e |= (p[1] & 0x3f) << 24;
            e |= (p[2] & 0x3f) << 18;
            e |= (p[3] & 0x3f) << 12;
            e |= (p[4] & 0x3f) << 6;
            e |= (p[5] & 0x3f);
            ub.bytes = 6;
        }
    } else if (*p >= 0xf8) {
        /* 5字节UTF-8: 111110aa 10bbbbbb 10cccccc 10dddddd 10eeeeee */
        if (p[1] < 0x80 || p[1] >= 0xc0 || p[2] < 0x80 || p[2] >= 0xc0 ||
            p[3] < 0x80 || p[3] >= 0xc0 || p[4] < 0x80 || p[4] >= 0xc0) {
            // 无效的UTF-8序列
        } else {
            e = (p[0] & 0x03) << 24;
            e |= (p[1] & 0x3f) << 18;
            e |= (p[2] & 0x3f) << 12;
            e |= (p[3] & 0x3f) << 6;
            e |= (p[4] & 0x3f);
            ub.bytes = 5;
        }
    } else if (*p >= 0xf0) {
        /* 4字节UTF-8: 11110aaa 10bbbbbb 10cccccc 10dddddd */
        if (p[1] < 0x80 || p[1] >= 0xc0 || p[2] < 0x80 || p[2] >= 0xc0 ||
            p[3] < 0x80 || p[3] >= 0xc0) {
            // 无效的UTF-8序列
        } else {
            e = (p[0] & 0x07) << 18;
            e |= (p[1] & 0x3f) << 12;
            e |= (p[2] & 0x3f) << 6;
            e |= (p[3] & 0x3f);
            ub.bytes = 4;
        }
    } else if (*p >= 0xe0) {
        /* 3字节UTF-8: 1110aaaa 10bbbbbb 10cccccc */
        if (p[1] < 0x80 || p[1] >= 0xc0 || p[2] < 0x80 || p[2] >= 0xc0) {
            // 无效的UTF-8序列
        } else {
            e = (p[0] & 0x0f) << 12;
            e |= (p[1] & 0x3f) << 6;
            e |= (p[2] & 0x3f);
            ub.bytes = 3;
        }
    } else if (*p >= 0xc0) {
        /* 2字节UTF-8: 110aaaaa 10bbbbbb */
        if (p[1] < 0x80 || p[1] >= 0xc0) {
            // 无效的UTF-8序列
        } else {
            e = (p[0] & 0x1f) << 6;
            e |= (p[1] & 0x3f);
            ub.bytes = 2;
        }
    } else if (*p >= 0x80) {
        // 首字节为10xxxxxx格式(0x80-0xBF)，是无效的UTF-8首字节
        // 首字节为10xxxxxx格式(0x80-0xBF)，是无效的UTF-8首字节，ub.bytes已初始化为0
    } else {
        /* 单字节ASCII: 0xxxxxxx (0-127) */
        e = p[0];
        ub.bytes = 1;
    }

    // 检查UTF-8编码有效性和范围
    if (ub.bytes == 0
        ||(ub.bytes > 1 && (*p == 0xc0 || *p == 0xc1)) // 禁止的过度编码
        ||(ub.unicode > 0x10FFFF) // Unicode超出有效范围
        ||(ub.unicode >= 0xD800 && ub.unicode <= 0xDFFF)) { // 代理区域字符
            ub.bytes = 0;
            ub.unicode = 0;
    } else {
        ub.unicode = e;
    }

    return ub;
}

/**
 * @brief an alias of from()
 * @param utf8_str UTF-8 string which to be converted
 * @returns GB2312 string
 */
String U2GB::get(String utf8_str) {
    return from(utf8_str);
}

/**
 * @brief convert UTF-8 string to GB2312 string
 * @param utf8_str UTF-8 string which to be converted
 * @returns GB2312 string
 */
String U2GB::from(String utf8_str) {
    Unibytes ub;
    String res = "";
    int i = 0;

    while (utf8_str[i]) {
        ub = getUnicodeChar(&utf8_str[i]);

        if (ub.bytes == 0) {
            // 处理无效UTF-8字符，使用空串代替
            i += 1; // 跳过当前无效字节
        } else {
            res += toGB2312(ub);
            i += ub.bytes;
        }
    }

    return res;
}

U2GB GB;

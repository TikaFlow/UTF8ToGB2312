#ifndef UTF8_TO_GB2312_H
#define UTF8_TO_GB2312_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef uint32_t ucs4_t;

typedef struct {
    ucs4_t unicode;
    int bytes;
} Unibytes;

#if !defined(NULL)
#define NULL ((void *)0)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

class U2GB {
private:
    static Unibytes getUnicodeChar(const char *str);

    static String toGB2312(Unibytes wc);

public:
    // an alias of from()
    static String get(const String &utf8_str);
    // convert from utf8 to gb2312
    static String from(const String &utf8_str);
};

#endif /* UTF8_TO_GB2312_H */

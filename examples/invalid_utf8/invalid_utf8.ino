#include "UTF8ToGB2312.h"

// 你?好
char str1[] = { 0XE4, 0XBD, 0XA0, 0X8B, 0XB4, 0XE5, 0XA5, 0XBD, 0x00 };
char str2[] = { 0XE4, 0XBD, 0XA0, 0XC0, 0XB4, 0XE5, 0XA5, 0XBD, 0x00 };

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(GB.get(str1)); // "你好" // 无效的字符被过滤 Invalid characters are filtered
  delay(1000);

  Serial.println(GB.get(str2)); // "你好" // Invalid characters are filtered
  delay(1000);
}
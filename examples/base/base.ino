#include "UTF8ToGB2312.h"

String str1 = "简体中文";
char str2[] = "中英文混合-Chinese and English mixed";
char* str3 = "测试字符串";  // 不建议 not recommended

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(GB.get(str1));
  delay(1000);

  Serial.println(GB.get(str2));
  delay(1000);

  Serial.println(GB.get(str3));
  delay(1000);
}
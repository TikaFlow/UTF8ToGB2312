#include "UTF8ToGB2312.h"

String str = "你——好";

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(GB.get(str)); // "你????好" // —未被GB2312收录
  delay(1000);
}
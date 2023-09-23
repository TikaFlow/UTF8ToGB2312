# UTF8ToGB2312

一个把UTF-8字符串转换成GB2312字符串的Arduino库。

> 已内置GB2312字库。

# 用法

1. 下载zip包。
2. 在Arduino IDE中[安装库](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries)。
3. 引入头文件: `#include "UTF8ToGB2312.h"`。
4. 默认有一个静态变量`GB`。
5. 通过`GB.get(<UTF-8字符串>);`语句将UTF-8字符串转换成GB2312字符串。
6. 更详细用法请参考[样例](examples)。

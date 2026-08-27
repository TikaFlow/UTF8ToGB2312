简体中文 | [English](README_EN.md)

# UTF8ToGB2312

一个把UTF-8字符串转换成GB2312字符串的Arduino库。

> 已内置GB2312字库。

# 用法

- ~~下载zip包。~~
- 在`Arduino IDE`中[安装库](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries)。
- 引入头文件: `#include "UTF8ToGB2312.h"`。
- 通过`U2GB::get(<UTF-8字符串>);`语句将`UTF-8`字符串转换成`GB2312`字符串。
- 更详细用法请参考[样例](examples)。

```cpp
#include "UTF8ToGB2312.h"

// 在setup或loop函数中
U2GB::from("你好，世界！");
// 或使用from方法的别名get方法
U2GB::get("你好，世界！");
```

# 注意

只能转换在`GB2312`中已收录的字符，其余字符无法转换，否则可能出现乱码。[#1](https://github.com/TikaFlow/UTF8ToGB2312/issues/1)

# 鸣谢

本库参考了 [char_conversion](https://github.com/bill611/char_conversion)。

其中 GB2312 字库表源自 [GNU libiconv](https://www.gnu.org/software/libiconv/)，遵循 LGPL-2.0-or-later 许可证。

English | [简体中文](README.md)

# UTF8ToGB2312

An Arduino library that converts UTF-8 string to GB2312 string.

> GB2312 library is included.

# Usage

- ~~Download zip file.~~
- [Install library](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries) in your `Arduino IDE`.
- Include head file: `#include "UTF8ToGB2312.h"`.
- Now you get a global variable `GB`.
- Convert `UTF-8` string to `GB2312` string by using `GB.get(<UTF-8 string>);`.
- See more in [examples](examples).

# Notice

Only the characters included in `GB2312` can be converted, the rest of the characters cannot be converted, or garbled characters may appear.[#1](https://github.com/TikaFlow/UTF8ToGB2312/issues/1)

# TinySjis2Utf8
Tiny ShiftJIS-to-UTF8 convert library\
軽量シフトJIS→UTF-8変換ライブラリ

[![PlatformIO CI](https://github.com/cat-in-136/TinySjis2Utf8/actions/workflows/platformio.yml/badge.svg)](https://github.com/cat-in-136/TinySjis2Utf8/actions/workflows/platformio.yml)

This library is intended to be used from PlatformIO.

Converts Shift JIS (CP932) encoded strings to UTF-8. A conversion table file (sjis2utf8.tbl) stored in external storage is required,
but some character types such as hiragana and katakana can be converted programmatically without referring to the conversion table file.

本ライブラリは PlatformIO から使うことを意図しています。

Shift JIS (CP932) でエンコードされた文字列を UTF-8 に変換します。
外部ストレージに保存された変換テーブルファイル (sjis2utf8.tbl) が必要ですが、
ひらがなカタカナなどの一部の文字種は変換テーブルファイルを参照せずにプログラムだけで変換します。

## How To Use / 使用方法

1. Download sjis2utf8.tbl from this repository and save this file to your SD card. / このレポジトリから sjis2utf8.tbl をダウンロードし、SDカードにこのファイルを保存します。
2. Add `https://github.com/cat_in_136/TinySjis2Utf8` to `lib_deps` in platformio.ini of your Platform IO project. / Platform IOプロジェクトのplatformio.iniの `lib_deps` に `https://github.com/cat_in_136/TinySjis2Utf8`を追加します
3. After opening the sjis2utf8.tbl file, call the tinysjis2utf8::sjis2utf8() function with arguments of it and a Shift JIS-encoded string. / sjis2utf8.tbl ファイルを開いた後、それと Shift JIS でエンコードされた文字列を tinysjis2utf8::sjis2utf8() 関数の引数に渡して呼び出します。

```c++
#include <SD.h>

// Convert S_JIS to UTF-8
File tbl_file = SD.open("/sjis2utf8.tbl");
String utf8_string = tinysjis2utf8::sjis2utf8(&tbl_file, sjis_string.c_str(), sjis_string.length());
tbl_file.close();
```


## LICENSE

MIT License

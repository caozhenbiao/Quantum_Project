# base-minimal

[![Build Status](https://travis-ci.org/zhchbin/chromium-base.svg?branch=master)](https://travis-ci.org/zhchbin/chromium-base)

Trimmed Chromium's base library, just add `base.gyp:base` to your gyp file to use them.

# To build

```bash
$ cd libbase
$ gyp --depth=. base.gyp -I build/common.gypi
$ ninja -C output
make
```
make builddir_name=output BUILDTYPE=Release


请使用 用 CMakeLists.txt

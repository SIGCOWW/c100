# C99 book
C99で発行(?:した|予定の)同人誌用リポジトリです。

## 執筆にあたって
GitHub Flowに従って執筆する。
```
# 執筆用ブランチの準備
$ git clone git@github.com:SIGCOWW/c99.git
$ git checkout -b (適当に決めた名前)

# 記事を書く
$ ./make.sh install kakuyo
$ vim src/articles/kakuyo/kakuyo.re
$ cp ~/dokokano/dir/hoge.png src/article/kakuyo/images/

# 必要に応じて commit/push
$ git add -A
$ git commit -m "hogehoge" # or git commit -m "[ci skip] TEST"
$ git push origin (さっき決めた名前)

# pullreq / [WIP]pullreq / draft pullreq を出す
# 全部終わったらSlackで伝えて他の人にマージしてもらう
```

Re:VIEW文法や注意については、[desk/template](https://github.com/SIGCOWW/desk/tree/master/template) に従う。

## ライセンス
本ディレクトリ以下は、[Beerwareライセンス](https://en.wikipedia.org/wiki/Beerware)のもとで提供されます。
```
/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* sigcoww@sigcoww.org wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return            SIGCOWW
* ----------------------------------------------------------------------------
*/
```

本ディレクトリ以下には、下記ソフトウェアの一部が含まれます。

### [template files for Re:VIEW](https://github.com/kmuto/review/tree/master/templates)
The MIT License ([LICENSE](https://github.com/kmuto/review/blob/master/templates/LICENSE))
```
Copyright (c) 2006-2016 Minero Aoki, Kenshi Muto, Masayoshi Takahashi, Masanori Kado.
```

### [ReVIEW-Template](https://github.com/TechBooster/ReVIEW-Template)
The MIT License
```
Copyright 2017 TechBooster

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

### [jsclasses](https://github.com/texjporg/jsclasses)
BSD 2-Clause "Simplified" License ([LICENSE](https://github.com/texjporg/jsclasses/blob/master/LICENSE))
```
pLaTeX2ε新ドキュメントクラス（日本語 TeX 開発コミュニティ版）
原作者：奥村晴彦 <okumura@okumuralab.org>

Copyright 1993-2016
The LaTeX3 Project and any individual authors listed elsewhere
in this file.

Copyright 1995-1999 ASCII Corporation.
Copyright 1999-2016 Haruhiko Okumura
Copyright 2016-2018 Japanese TeX Development Community
```

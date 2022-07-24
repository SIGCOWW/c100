= ねっとりNetlink
#@#@<author>{:>=, lrks;twitter:__lrks__}
#@#//profile{
#@#C97では「次はSquare Standのアプリ連携周りを記事にしたい」と思っていましたが、この2年間何もしていないので没になりました。
#@#次はSquare Terminalのアプリ連携周りを紹介したいです。
#@#//}
//lead{
Linuxカーネルとユーザランドのコミュニケーションに使われる「Netlink」について紹介します。
これを読めばDXの本質が@<ruby>{理解,わか}らせられます。
NTR要素、戯言シリーズ要素はありません。
//}

== Netlinkとは
包括的に言えば「Linuxカーネルとユーザランドのプロセス間通信の仕組み」です。
ipコマンドを実行しようとして、～～のような応答が帰ってくるのを見かけた人も居るのでは？
これはipコマンド（ユーザランドのプログラム）がNetlink経由でカーネルにNetwork Interfaceを追加しようとしたところ、Errorが返された、理由は実はすでに同名のインターフェースが登録されていたということを示しています。

ipコマンドだったらNetworkっぽいので、「Net」linkという名前に納得です。
でも実はNetwork以外にも使える、それはtypeで見分ける。
…とはいえNetworkまわりに特化している部分もあるんですけどね、詳細は後述します。





かきくけこ

== netlink type
typeの実際の値はどこを見ればよいか

== rtnetlink
hogehoge

== debug
ip monitor
nlmon
open netlink socket

== 俺たちで調べてみないか？netlinkのこともっと
これ以上の詳細は～～へ

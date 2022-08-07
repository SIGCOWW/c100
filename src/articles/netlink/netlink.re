= ねっとりNetlink
@<author>{WSSがURI Schemeにしか見えない, lrks}
//lead{
ねっとり霊夢です。ねっとり魔理沙だぜ。今日はNetlinkについて紹介していくぜ。
これを読めばWeb3がいちばんやさしく@<ruby>{理解,わか}らせられるって話だぜ。
//}

== Netlinkとは
本稿ではNetlinkについて紹介していくぜ。
出典はRFC 3549@<fn>{rfc3549}および@<code>{man 7 netlink}、@<code>{man 7 rtnetlink}です。
//footnote[rfc3549][J.Salim, H.Khosravi, A.Kleen, A.Kuznetsov. ``Linux Netlink as an IP Services Protocol'', RFC 3549.]

まずNetlinkとは何か（ねっとり）という話です。
Netlinkとは「Linuxカーネルとユーザランドのプロセス間通信」です。
端的に換言せずともIPC (Interprocess Communication)。
@<tt>{AF_NETLINK}に対してSocketを作ることでNetlinkに接続できます。

たとえば、ipコマンドを利用して``lo''@<fn>{lo}という名前のdummyインターフェースを追加してみましょう。
実際実行してみると次のようになると思います。
//cmd{
$ sudo ip link add lo type dummy
RTNETLINK answers: File exists
//}
大抵は同名のLoopbackインターフェースが存在するため作成できません。
でもよく見たら、RT@<b>{NETLINK}って書いていませんか？
はい、ipコマンドはNetlinkを使います。
前述の出力はipコマンド（ユーザランドのプログラム）がカーネルにNetlink経由で接続しdummyインターフェースの作成を指示した、
がカーネル側はすでに同名のインターフェースが存在したため作成に失敗したことを通知しています。
//footnote[lo][名前に意味はありません。偶然だぞ。]

でも、RTNETLINK？Netlinkの話なのになんで先頭に@<b>{RT}があんだよ、教えはどうなってんだ教えはって話です。
ここで@<bou>{RT}はRoutingのことを指します。
Routingと@<b>{Net}linkなんてすごく関係していそうですね。
Netlinkにはいくつか種別があり、その中のひとつがRTNETLINKです。
他の種別、例えばnetfilter関連のNETLINK_NETFILTERや本当にLinuxカーネルとユーザランドのIPCとして利用可能なNETLINK_GENERICもあります。
が、それらは割愛して本稿ではRTNETLINKを中心に紹介していきます。

ちなみに今回の説明ではipコマンドを使いましたが、ifconfigコマンドだとどうなるのでしょうか？
ifconfigではprocfsとioctlに基づいてカーネルとやりとりしするためNetlinkは関係ありません。

== Netlinkの繋ぎ方
Netlinkには@<b>{FEC} (Forwarding Engine Component) と@<b>{CPC} (Control Plane Component) という概念が存在します。
FECは典型的にはカーネル側の話、RTNETLINKなどNetlinkの1種別のことです。
CPCはユーザランドのプログラムを指します。
@<img>{logical}のようにFECとCPCは@<b>{Wire}で繋がり、FECとCPCは1対多で接続可能、CPCは複数のWireに接続可能です。
例を挙げると、RTNETLINKという1つのFECに対して複数のipコマンドが接続でき、
1つのipコマンドはRTNETLINK以外にも他のNetlink種別に接続しようと思えばできます。

//image[logical][Netlink Logical Model（RFC3549より引用）][scale=0.7]

実際の接続は@<tt>{socket(2)}と@<tt>{bind(2)}で行います。
次のコードはユーザランドのプログラムからFECに接続するイメージと@<tt>{struct sockaddr_nl}の定義です。
//emlist{
int fd = socket(AF_NETLINK, SOCK_RAW, <FEC番号>);
bind(fd, struct sockaddr_nl *sa, socklen_t sa_len);

struct sockaddr_nl {
  sa_family_t     nl_family; /* AF_NETLINK */
  unsigned short  nl_pad;
  pid_t           nl_pid;
  __u32           nl_groups;
};
//}

接(頭|尾)辞に使われる@<tt>{nl}はNetlinkのことを指します。
@<tt>{sockaddr_nl}の@<tt>{nl_pid}は宛先のPort IDまたはProcess IDです。
カーネル宛の場合は@<tt>{0}になります。
@<tt>{nl_groups}はMulticast Group (Bitmask)の指定です。

NetlinkではMulticastメッセージが流れる@<fn>{multicast}ことがあり、
@<tt>{nl_groups}で何のメッセージを受け取るか？または受け取らないか指定できます。
たとえば、FECにRTNETLINKを指定して@<tt>{nl_groups}を@<tt>{RTMGRP_LINK | RTMGRP_NEIGH}とすると、
NICのLink状態またはip neigh (ARP Table) が変わった際に通知を受けられます@<fn>{rtnetlinkh}。
便利ですね、この通知が受けられるというのはこれだけでNetlinkを利用する価値のある機能だと思います。
（というか他に実現する方法あるのかな…？）
この機能があることで、例えばDPDKなどで実現され本来カーネルのFIBを見ないようなアプリケーションでもカーネルの内容と同期するようなことが可能です。
//footnote[multicast][CAP_NET_ADMIN権限があればWireに対してMulticastメッセージを流すこともできます。とはいえカーネル以外でメッセージを流したい場合はあるんですかね…？]
//footnote[rtnetlinkh][他の値はrtnetlink.hにあります @<href>{https://github.com/torvalds/linux/blob/e2b542100719a93f8cdf6d90185410d38a57a4c1/include/uapi/linux/rtnetlink.h#L677-L695}]

なお、とりあえずRTNETLINKで流れてくるメッセージが見たいという場合は@<code>{ip monitor}コマンドで確認できます。
//cmd{
$ ip monitor
fe80::812:1 dev ens3 lladdr xx:xx:xx:a0:00:6f router REACHABLE
xxx.xx.xxx.x dev ens3 lladdr xx:xx:xx:9f:f0:0b PROBE
xxx.xx.xxx.x dev ens3 lladdr xx:xx:xx:9f:f0:0b REACHABLE
fe80::812:3 dev ens3 lladdr xx:xx:xx:42:72:17 router STALE
:
^C
//}
何かメッセージを受信するたびに1行ずつ出力されていきます。
何を受信したときにどんな動きをしたいかにもよりますが、非常に簡単なものの場合は自前でRTNETLINKに接続せずとも、
この出力をパースすることでNICのUp/Downに反応するなどはできそうです。

ちなみにRTNETLINKに限らずNetlink全体のメッセージが見たい場合は、nlmon@<fn>{nlmon}やnltrace@<fn>{nltrace}も使えます。
nlmonで取ったデータはWiresharkで表示可能です。Packet DiagramもMessage Header（後述）くらいは出てきます。
//footnote[nlmon][netlinkファンのためのnlmon - Qiita @<href>{https://qiita.com/kwi/items/991c3bd01889db45307e}]
//footnote[nltrace][@<href>{https://github.com/socketpair/nltrace}]

== Message Format
Netlink Socketに対し、メッセージを送受信することでFECに指示または情報を受け取れます。
ここではそのメッセージのFormatについて扱います。

メッセージは、1つ以上の「@<b>{Netlink Message Header}とペイロード（各FECのデータ）」で構成されます。
ペイロードが大きい場合は複数のメッセージ (Multipart Message) に分割可能です。

=== Netlink Message Header
まずNetlink Message Headerの構成は、@<img>{msghdr}のとおりです。

//imagew[msghdr][Netlink Message Header（RFC3549を基に作成）][scale=1]

ここでTypeには次のような値が入ります。

: NLMSG_NOOP
  メッセージを処理しないようにする。
: NLMSG_ERROR
  エラーメッセージがある。
  一般的にはFEC（カーネル）からCPC（ユーザランド）に送られる。
  後続のError Codeが0ならエラーではない（FlagsにNLM_F_ACKを指定し、処理が成功した場合ACKとして利用される）
: NLMSG_DONE
  Multipart Messageの終端を示す。
  終端以外のMultipart MessageはFlags側で制御

なお、TypeはBitmaskではなく複数のTypeが同時に指定されることはありません。
また、FECによってはここを拡張することもあります。
例えばRTNETLINKなら、RTM_NEWLINK、RTM_DELLINK、RTM_GETLINKといったTypeが増えます。

Flags (Bitmask) に次のようなFlagが入ります。

: NLM_F_REQUEST
  リクエストメッセージ。
  一般的にはCPC（ユーザランド）からFEC（カーネル）に送られる。
: NLM_F_MULTI
  終端以外のMultipart Message
: NLM_F_ACK
  処理成功時にACKを要求する。
  一般的にはCPC（ユーザランド）からFEC（カーネル）に送られる。
: NLM_F_ECHO
  エコーリクエスト。
  一般的にはCPC（ユーザランド）からFEC（カーネル）に送られる。
  TypeにNLMSG_NOOPと組み合わせてハートビートが可能。

FECがRTNETLINKで、TypeにRTM_GETLINKなどをGET系だった場合、次のFlagも使われます。

: NLM_F_ROOT
  単一のエントリではなく完全なテーブルを返す
: NLM_F_MATCH
  内容に一致したすべてのエントリを返す

具体的な使われた方としては次のとおりです。
//cmd{
# flags = NLM_F_REQUEST | NLM_F_ROOT | NLM_F_MATCH
$ ip route

# flags = NLM_F_REQUEST
$ $ ip route get <特定のネットワーク>
//}

また、TypeにRTM_NEWLINKなどNEW系を指定した場合は次も使われます。

: NLM_F_REPLACE
  条件に一致したエントリをリクエストの内容で上書きする
: NLM_F_EXCL
  すでにエントリが存在した場合は上書きしない
: NLM_F_CREATE
  エントリが存在しないなら作成する
: NLM_F_APPEND
  エントリに追記する

具体的には次のとおりです。
//cmd{
# NLM_F_CREATE -> OK
$ ip route add 192.168.1.0/24 nexthop via 169.254.0.1 dev eth0

# NLM_F_CREATE -> すでにeth0を使うとしているのでNG
$ ip route add 192.168.1.0/24 nexthop via 169.254.0.1 dev eth1
RTNETLINK answers: File exists

# NLM_F_APPEND -> これならOK（eth0側のRouteも残る）
$ ip route append 192.168.1.0/24 nexthop via 169.254.0.1 dev eth1

# NLM_F_REPLACE -> これでもOK（eth0側のRouteは上書きされる）
$ ip route replace 192.168.1.0/24 nexthop via 169.254.0.1 dev eth1
//}

=== ペイロード
RTNETLINKを利用する場合のペイロードについてです。

ペイロードは基本的にはTLV (Type-Length-Value) 形式…らしいのですが、特殊です。
ペイロードの中にもヘッダがある+後続にAttributeと呼ばれるデータが続くような形式で、
友好的に解釈しヘッダ自体を``T''の一種とみなし、``V''の中にさらにTLVが入れ子になっていてもセーフとすればTLVです。
ちょっとややこしいですが一応パース@<fn>{macro}に必要な情報は揃っています。
//footnote[macro][実際にパースする際は@<tt>{rtnetlink.h}に定義される@<tt>{RTA_}系のマクロを使うことになるはずです。]

以下からは各``T''とAttributeについて詳細を述べていきます。

==== Link
NICの作成削除、Link Up/Downといった状態の取得や変更を行う場合、@<img>{link}のヘッダが利用されます。

//imagew[link][Network Interface Service ModuleのHeader（RFC3549を基に作成）][scale=1]

Netlink Message HeaderのTypeにRTM_NEWLINK、RTM_DELLINK、RTM_GETLINKのいずれかが入ってきたらこのペイロードが付いています。
Device FlagsはNETDEVICEのflags@<fn>{netdevice}です。
//footnote[netdevice][@<href>{https://github.com/spotify/linux/blob/6eb782fc88d11b9f40f3d1d714531f22c57b39f9/include/linux/if.h#L30-L57}]

そして、この@<img>{link}のあとにAttributeが続きます。
Attributeの構造は@<tt>{rtattr}のとおりTLV@<fn>{ltv}です。
//emlist{
struct rtattr {
  unsigned short rta_len;
  unsigned short rta_type;
  /* Value */
};
//}
この@<tt>{rtattr}は複数個がくっついて送られる場合もあります。
連続して付いても、Lengthの情報はあるのでパースは出来ます。
//footnote[ltv][Length, Type, Valueの順なので正確には@<b>{LTV}とでも呼んだら良いでしょうか。]

実際どんなAttributeがあるかですが、Linkの場合
例えば@<tt>{rta_type}がIFLA_ADDRESSに設定されValueがL2アドレスだったり、
typeがIFLA_MTUでValueは名前通りMTUだったりします。

他にはどんなtypeが設定され、どんなValueを持つかについては@<code>{man 7 rtnetlink}に記載されています。
が、ソースコードの@<tt>{do_setlink}@<fn>{do_setlink}からも確認できそうです。
//footnote[do_setlink][@<href>{https://github.com/torvalds/linux/blob/b44f2fd87919b5ae6e1756d4c7ba2cbba22238e1/net/core/rtnetlink.c#L2650}]

==== Neighbor (ARP)
ARPエントリに関するメッセージは@<img>{neighbor}が利用されます。

//imagew[neighbor][Neighbor Setup Service ModuleのHeader（RFC3549を基に作成）][scale=1]

Netlink Message HeaderのTypeはRTM_NEWNEIGHなどNEIGH系です。
この後にも@<tt>{rtattr}が続き、その@<tt>{rta_type}は例えばNDA_DST（IPアドレスについて）やNDA_LLADDR（MACアドレスについて）となります。

==== Address
つぎは（IP）アドレス系です。図は@<img>{ipaddr}です。

//imagew[ipaddr][IP Address Service ModuleのHeader（RFC3549を基に作成）][scale=1]

Netlink Message HeaderのTypeはADDR系です。
もちろん@<tt>{rtattr}も続き、IPアドレスやブロードキャストアドレスが入ります。

==== Routing
Routing系です。@<img>{routing}。

//imagew[routing][Network Route Service ModuleのHeader（RFC3549を基に作成）][scale=1]

Netlink Message HeaderのTypeはROUTE系です。
あの@<tt>{rtattr}も続き、宛先経路のアドレスやゲートウェイの情報が入ります。

ROUTE系は他のものに比べ、Attributeが多いです。
RTA_MULTIPATHというAttributesもあるのですが、このパースは複雑で混乱しました。
MULTIPATHというだけあって長さは可変長になりそうですがその通りで、
このRTA_MULTIPATHの中でヘッダとは別にさらに@<tt>{rtattr} (RTA_GATEWAY) が繰り返されるような形です。
このRTA_GATEWAY自体も固定長ではなくIPv4とIPv6の場合で長さが変わり、当然Multipath中にIPv4,IPv6のGATEWAYが混在する可能性もあります。
元々Netlinkのペイロード自体がTLVでその中のVがTLVでさらにその中のVがTVLで…全体的にはTL(TL(TLV)*)的な、え？最後TLVだった？いやVLT？？？といま思い出そうとしてもあまり考えずにいたいところです。
もう考えたくありません。出来たから良いんです。

==== その他
Link、Neighbor、Address、Routingについて紹介しました。
このほか、Traffic Controlに関するメッセージもあるのですが使わなかったため省略します。


== まとめ
NetlinkとRTNETLINKについてまとめました。

実は元ネタは社内勉強会の資料です@<fn>{dojin}。
当時作成した際は記憶が鮮明でしたが時間が経つに連れて記憶が薄れていっているところ、今回改めて理解を深める機会となり良かった。
//footnote[dojin][今回の同人誌は当然プライベートの時間で作成しました。社内限定の資料も参照していません。本当です。でも自分が休暇中に作成し社内で共有した資料は参照しました。すみません。]

その勉強会でまとめた動機としては、すでにNetlinkとRTNETLINKを使っているプログラムがあり、
そのプログラムで新たなNetlink Messageを読むためNetlinkの理解を深めるというものでした。
そのためイチからNetlinkにSocket張ったり、ライブラリ (libnetlink) については本稿でも端折り気味です。
もし今後、今回の内容が再録されるときにはまとめたりまとめなかったりしようと思います。
おわり。

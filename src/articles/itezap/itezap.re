= 目に注目したイラストレーター識別器の検討
@<author>{学位取得は遅れがち, itezaP;twitter:sugerest}

//profile{
C100めでたすぎだろ！
//}

//lead{
目標はこぶいち・むりりん判別器です
//}

== ご挨拶

こんにちは、itezaPと申します。
ひとまず、無事（?）にC100を迎えられたこと大変めでたく思います。
私はC84からの参加と新参者でありますが、いつも楽しく参加させていただいています。

新しい記事については2019年の冬コミ以来約2年半ぶりになります。
今回の記事は、キャライラストの目だけを使ったイラストレーター識別器を試作しました。
具体的には、深層学習が目の画像だけで特定のイラストレーターを識別できるか、を試した記事になります。

== はじめに

あるキャラのイラストを見たときに「この描き方どこかで見たことあるんだよな」と思ったことはありますか。
私はよくあります。
そのような感覚は、どの部分を見て感じることが多いでしょうか。
例えば「目や瞳の描き方」「髪の塗り方」「顔の輪郭」などが挙げられます。
特にわかりやすいのが目の描き方ではないでしょうか。
例として@<img>{comp_illust}に、あるイラストから目の部分のみを切り抜いたものになります。
図中左の2枚は比較的区別しやすく、右の2枚は細かく見ないと区別しにくいと思います。
見分けるポイントは、黒目の描き方やグラデーションの塗り方などの瞳の描き方です。
このように「見たことあるけど名前が思い出せない」や「この描き方好きだから、似た人いないかな」という画像検索がしたいことがあります。

//image[comp_illust][判別しやすい目と判別しにくい目の比較][scale=0.8]

異なる画像同士の類似度を計算する手法は数多く提案されています。
類似度は全体の色合いや物体の輪郭など、比較したい種類に応じて定義されます。
類似度はこれらの特徴を、何らかの数値やベクトル、特徴量として定義します。
そして、それらの距離を計算することで求めることができます。
現実の画像検索サービスとして、Google画像検索が挙げられます。
もう一つ、画像の直接比較によらない方法の一つとして、タグ付けによる類似画像検索が挙げられます。
様々なサイトが行なっていますが、イラストだとPixivやニコニコ静画などが有名です。
画像比較の方が類似した画像を検出できますが、膨大な画像数に対して類似検索を行うと計算量が膨大になります。
また上記にも挙げたように、類似度が多様なため定義が困難です。

新たなイラスト類似検索の試みとして、今回はイラストの目に注目したイラストレーター識別器の試作を開発してみました。

== 方法

===　畳み込みニューラルネットワーク（CNN）

画像認識の分野では、深層学習による識別や分類手法が大きな成功を収めています。
今回は、よく利用される古典的な深層学習である畳み込みニューラルネットワーク（CNN）を利用しました。
CNNの仕組みについては、さまざまな本やサイトがあるのでここでの詳細は割愛させていただきます。

今回CNNは4層の畳み込みレイヤーと4層の全結合層から構成されます。
出力層の活性化関数にはsigmoid関数を使うことで二値分類を行いました。
入力は100×100×3のカラー画像として、出力は予測ラベルです。
今回は計算の簡略化のため入力画像サイズを統一しました。
参考として、ネットワークの概要を@<img>{network_summary}に掲載します。

//image[network_summary][ネットワークの概要][scale=0.5]

=== データセット

データセットはいくつかのイラストレーターを対象に、ゲーム公式サイトや検索によって得られたキャラクターイラストを使用しました。
さらに、画像データは事前に目周辺の部分のみを切り抜き、右目と左目を別画像として保存しました。
また、目が閉じてあるものは除外しました。
@<img>{comp_illust}に掲載しているものは、データセット内の画像を利用しています。
このように目周辺だけ切り抜いた画像を約300枚用意しました。
学習に応じて画像データは、0または1でラベリングされます。
特定のイラストレーターを識別したい場合、そのイラストレーターが描いた画像に対して1、それ以外は0をラベリングします。
実験データは9割中8割を訓練データとし1割を検証データ、残りの1割をテストデータとしました。
データはモデル実行毎にランダムに選択するようにしました。
今回使用したデータセットの絵師とその枚数を以下の@<table>{dataset}に示します。

//tabooular[dataset][データセット内訳]{
名義（敬称略）	|	枚数
==========>|====
みつみ美里	|	79
甘露樹	|	62
カワタヒサシ	|	60
なかむらたけし	|	24
樋上いたる	|	8
梱枝りこ	|	26
しらたま	|	21
和泉つばす	|	7
七尾奈留	|	8
（合計）	|	295
//}

== 結果と考察

=== 実験1：特定イラストレーターの識別

はじめに特定のイラストレーターを識別できるかを実験しました。
この実験ではみつみ美里さんを対象としました。
各実験はepoch（学習回数）を40として、それを独立に５回繰り返した結果の平均と分散を出力しました。
以降の実験では、特筆しない限りすべて同様の条件で行いました。

結果は平均0.889、分散0.001となりました。
分散は簡単に言うと、各実験の精度が平均からどれくらい離れているかを表しています。
結果として、9割近く正確にイラストレーターを識別できました。
予測ラベルが間違っていたいくつかの画像を@<img>{mistake_predict_1}に示します。
誤認識した画像（@<img>{mistake_predict_1}上段）を見ると、人目で見た限りでは識別できそうなものが誤認識されています。
違いとして瞳の描き方が考えられますが、これらはかなり似通っているのため、まだ十分に学習できていない可能性があります。
本人ではないと認識した画像（@<img>{mistake_predict_1}下段）に注目すると、描かれた時代が前の画像（中央と右の画像）のものが見られました。
特に右の画像はToHeart2のキャラクターで、現在の描き方（左の画像を参照）と大きく異なることがわかります。
この時代の画像は少なく、うまく学習できなかったと考えられます。

//image[mistake_predict_1][誤認識した画像の一例][scale=0.8]

=== 実験2：カラー情報を削除した場合の識別

イラストを見分ける基準の一つとして、黒目や瞳の描き方が重要であると考えられます。
さらに、そこに色の使い方も重要であると考えました。
そこでカラー情報を落とした白黒画像での識別を行いました。

結果は平均0.848、分散0.006でした。
カラー情報がある実験1と比べて、約5％精度が落ちました。
白黒画像で失敗したいくつかの画像を@<img>{mistake_predict_2}に示します。
失敗した原因の一つとして、光源による瞳の中の白い部分と瞳の中のグラデーションの区別がしにくくなったのではと考えられます。
誤認識した画像（@<img>{mistake_predict_2}上段、右の画像）を確認します。
この画像はかなりみつみ美里さんに似ていることが見て取れます。
カラー情報を削除することで、色の塗り方に関する何らかの識別特徴が失われ、誤認識した可能性があります。
結果として、形やグラデーションだけである程度識別できるが、さらに精度が高めるためにカラー情報を考慮する必要があることがわかりました。
今回はみつみさんに限定した識別ですが、塗り方もイラストレーターの個性のため、カラー情報は共通で必要になると考えられます。

//image[mistake_predict_2][白黒画像によって誤認識した画像の一例][scale=0.8]

=== 実験3：同イラストレーター画像だけを用いた年代識別

実験1では古いキャラクターは誤認識する結果が得られました。
そこで、同じイラストレーターの画像に限定して識別できるかを調査しました。
データはみつみ美里さんのものに限定し、作品の年代によってラベリングして実験しました。
具体的にはダンジョントラベラーズ2を含んだ以前の作品を0、それ以降の作品を1としました。
データ内訳はそれぞれ0が42枚、1が37枚になります。

結果は平均0.885、分散0.003となりました。
こちらも精度は約9割と高い精度を出すことができました。
同様に予測を間違えた画像を@<img>{mistake_predict_3}に示します。
過去の絵柄と誤認識した例を見ると、瞳の描き方が少し特徴が異なることが見て取れます。
そのため別クラスとして誤認識されたと考えられます。
最近の絵柄と誤認識した例では、前回の実験と同様にToHeart2のキャラクターが出てきました。
やはりこの時代のデータが少なくため、うまく学習できていないことが考えられます。
0でラベリングしたデータのうち、約8割はダンジョントラベラーズ2のキャラクターです。
ダンジョントラベラーズ2は2013年発売のため、描き方は近年の描き方（クラス1）にそれなりに近いです。
正しく認識させるには、ToHeart2や1999年に発売されたこみっくパーティ時代の画像の充実が課題です。

//image[mistake_predict_3][年代を誤認識した画像の一例][scale=0.8]

== おわりに

今回は目に注目したCNNによるイラストレーター識別器の試作を行いました。
簡単なネットワーク構造でも精度は約9割で、それなりの高精度になることが示唆されました。
今回は試作と時間の都合上、しっかりとした精度計測やパラメーターチューニングをしていません。
ネットワーク構造も全然最適化できていません。
次回の検討事項とさせていただきます（予定は未定）。

今後はデータセットの拡充が考えられます。
具体的にはイラストレーターの種類を増やしたり、古い年代の画像を追加したいです。
そのために最近ToHeart2DXを買いました（隙自語）。
また、現在データの作成（目の切り抜き、左右の目の分離）が手動なので作成に時間がかかります。
この作業も可能であれば自動化したいです。
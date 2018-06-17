【ソフト名】 anipmaker
【著作権者】 Copyright (C) 2018 tapetums
【種　　別】 WebPアニメーション作成アプリケーション
【動作環境】 Windows 7/8/8.1/10 32/64-bit
【開発環境】 Visual Studio Community 2017


【概要】
WebPアニメーション作成アプリケーションです。

Google の開発した libwebp をライブラリとして使用しています。
https://github.com/webmproject/libwebp


【アーカイブ内容】
├─README.TXT
├─AUTHORS
├─COPYING
├─LICENSE
├─PATENTES
├─x64
│　└─anipmaker.exe
└─x86
　　└─anipmaker.exe

計 7 ファイル


【インストール】
任意のフォルダに展開して下さい。
レジストリは使用していません。

お使いの OS が 32-bit版 の場合は x86\anipmaker.exe を、
64-bit版 の場合は x64\anipmaker.exe を それぞれお使いください。


【アンインストール】
配置したフォルダごと削除して下さい。


【設定】
loop count
ループ回数を指定します。0で無限ループになります。

minimize
ファイルサイズを最小化します。エンコーディング速度が低下します。

kmin / kmax
キーフレームの距離を設定します。※現バージョンでは設定できません

lossy / lossless
不可逆圧縮／可逆圧縮を選択します。

quality
圧縮品質を設定します。[0..100]

method
圧縮方法を選択します。6が最も画質が良くなりますが、最もエンコーディング速度が遅くなります。[0..6]

image hint
圧縮のヒントとなる画像の種類を指定します。lossless のみで有効です。

target size
エンコーディング後のサイズを指定します。圧縮設定よりも優先されます。0だと無視されます。

PSNR
ピーク信号対雑音比を設定します。target size よりも優先されます。0だと無視されます。

segments
最大セグメント数を設定します。デフォルトは 4 です。[1..4]

SNS strength
Spatial Noise Shaping を設定します。デフォルトは 50 です。[0..100]

filter strength
フィルタの強さを指定します。デフォルトは 60 です。[0..100]

filter sharpness
シャープネスの強度を指定します。デフォルトは 0 です。[0..7]

filter type
フィルタの種類を指定します。autofilter かつ filter strength > 0 のときのみ使用されます。

autofilter
フィルタの強さを自動調整します。

alpha compression
アルファチャネルに対する圧縮設定です。lossless でのみ有効です。

alpha filtering
アルファチャネルに対するフィルタの設定です。

alpha quality
アルファチャネルの品質を設定します。デフォルトでは 100 です。[0..100]

pass
解析パスの回数を設定します。デフォルトでは 1 です。[1..10]

preprocessing
圧縮前処理の方法です。

partitions
[0..3] 

partition limit
[0..100] 

emulate jpeg size
圧縮後のサイズを JPEG を基準に計算します。

thread level
マルチスレッド処理を試みます。

low memory
CPUパワーを犠牲にして省メモリ化を試みます。

near lossless
ニアロスレス圧縮の品質を設定します。小さいほど高圧縮率です。デフォルトでは 100 です。[0..100]

exact
透過エリアにおけるRGB値を保存します。

frame skip
フレームを飛ばしてフレームレートを落とします。


【ライセンス】
NYSL Version 0.9982 です。このプラグラムを使用したことによる一切の影響は関知しません。
http://www.kmonos.net/nysl/

libwebp を使用しています。
詳しくは AUTHORS, COPYING および PATENTS を参照してください。


【更新履歴】
2018.06.17 v0.1.0.0
 ・初版作成


【文責】
tapetums
https://github.com/tapetums
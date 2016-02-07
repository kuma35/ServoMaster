.. -*- coding: utf-8; mode: rst; -*-

.. include:: definition.txt

=====
Stack
=====

Stackクラスです。現在は整数値のみ格納可能です。
Arduino標準では例外処理を使わない設定になっているので、
スタックオーバーフローやスタックアンダーフローを例外処理で扱う訳には
いきません。

そこでこのStackクラスではpush,popの際に必ず検査するコードを入れることにより
これらを処理します。pushable,popableがそれです。







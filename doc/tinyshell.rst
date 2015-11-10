.. -*- coding: utf-8; mode: rst; -*-

.. include:: definition.txt

=========
TinyShell
=========

TinyShellクラスライブラリはSerial経由およびI2C経由で得られた文字列を解釈・実行します。

メソッド
========

get_line
	Serialから文字列を読み込みます。NEWLINEで終了する前提で動作します。
get_token
	得られた文字列からトークンを切り出します。
get_number
	得られたトークンが数値であるかどうかを判定し、数値の場合はスタックに積みます。
execute
	切り出したトークンの実行を試みます。



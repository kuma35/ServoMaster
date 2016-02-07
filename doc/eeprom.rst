.. -*- coding: utf-8; mode: rst; -*-

.. include:: definition.txt

======
EEPROM
======

ServoMasterでは各スケッチが起動時にEEPROMを読み取りI2Cマスター/スレーブ、
各サーボ操作のデジタルポートピン番号、各サーボの動作範囲(最小〜最大)を
設定します。また、ボードによってはサーボ全体のON/OFF制御ポート番号を保持します。

ServoMasterでは以下のメモリマップで運用するものとします。

.. list-table:: EEPROMメモリーマップ

		* - アドレス
		- 長さ
		  
		  
  
     

EEPROMへの書き込みは手動コマンドで行います。eeprom-writeワードを使います。






